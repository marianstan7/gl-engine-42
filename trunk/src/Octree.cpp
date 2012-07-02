//
// Octree.cpp for glengine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat May  5 10:59:21 2012 loick michard
// Last update Tue Jun 26 18:58:16 2012 gael jochaud-du-plessix
//

#include <Octree.hpp>
#include <Mesh.hpp>
#include <Scene.hpp>
#include <Geometries.hpp>

gle::Octree::Node::Node(const Vector3<GLfloat>& min,
			const Vector3<GLfloat>& max,
			const std::list<Element*> &elements,
			const std::list<Element*> &partialsElements) :
  _elements(elements), _partialsElements(partialsElements),
  _debugMaterial(NULL), _debugMesh(NULL), _leaf(true)
{
  for (int i = 0; i < 8; ++i)
    _children[i] = NULL;
  _min = min;
  _max = max;
}

gle::Octree::Node::~Node()
{
  delete _debugMesh;
  delete _debugMaterial;
}

gle::Mesh* gle::Octree::Node::getDebugMesh()
{
  if (!_debugMaterial)
    {
      _debugMaterial = new Material();
      _debugMaterial->setAmbientColor(gle::Color<GLfloat>(0, 1.0, 0));
    }
  if (!_debugMesh)
    {
      _debugMesh = gle::Geometries::Cuboid(_debugMaterial,
					   (_max.x - _min.x > 0) ? _max.x - _min.x : _min.x - _max.x,
					   (_max.y - _min.y > 0) ? _max.y - _min.y : _min.y - _max.y,
					   (_max.z - _min.z > 0) ? _max.z - _min.z : _min.z - _max.z, true);
      Vector3<GLfloat> _center = (_max + _min);
      _center /= 2.0;
      _debugMesh->setPosition(_center);
      _debugMesh->setRasterizationMode(gle::Mesh::Line);
    }
  return (_debugMesh);
}

gle::Octree::Octree() : _root(NULL)
{

}

gle::Octree::~Octree()
{

}

void gle::Octree::generateTree(std::list<Element*> &elements)
{
  Vector3<GLfloat>	_min;
  Vector3<GLfloat>	_max;
  unsigned int		i = 0;

  for (Element* &element : elements)
    {
      const gle::Vector3<GLfloat>& min = element->getMinPoint();
      const gle::Vector3<GLfloat>& max = element->getMaxPoint();
      if (i == 0 || min.x < _min.x)
	_min.x = min.x;
      if (i == 0 || min.y < _min.y)
	_min.y = min.y;
      if (i == 0 || min.z < _min.z)
	_min.z = min.z;
      if (i == 0 || max.x > _max.x)
	_max.x = max.x;
      if (i == 0 || max.y > _max.y)
        _max.y = max.y;
      if (i == 0 || max.z > _max.z)
        _max.z = max.z;
      ++i;
    }
  _root = new Node(_min, _max, elements, {});
  _nbThreads = 0;
  _root->splitNode(false, &_nbThreads, 8);
}

static void split(gle::Octree::Node *node, bool thread, std::atomic<int> *nbThreads, int maxThreads, int depth)
{
  node->splitNode(thread, nbThreads, maxThreads, depth);
}

void gle::Octree::Node::splitNode(bool thread, std::atomic<int> *nbThreads, int maxThreads, int depth)
{
  struct subdivision {
    gle::Vector3<GLfloat> min;
    gle::Vector3<GLfloat> max;
    std::list<gle::Octree::Element*> elements;
    std::list<gle::Octree::Element*> partialsElements;
  };

  if (_elements.size() <= 8)
    {
      if (thread)
	(*nbThreads)--;
      return ;
    }

  subdivision subdivisions[8] = 
    {
      {
	{(_min.x + _max.x) / 2, (_min.y + _max.y) / 2, (_min.z + _max.z) / 2},
	{_max.x, _max.y, _max.z},
	{}, {}
      },
      {
        {(_min.x + _max.x) / 2, _min.y, (_min.z + _max.z) / 2},
	{_max.x, (_min.y + _max.y) / 2, _max.z},
        {}, {}
      },
      {
        {(_min.x + _max.x) / 2, (_min.y + _max.y) / 2, _min.z},
	{_max.x, _max.y, (_min.z + _max.z) / 2},
        {}, {}
      },
      {
        {(_min.x + _max.x) / 2, _min.y, _min.z},
        {_max.x, (_min.y + _max.y) / 2, (_min.z + _max.z) / 2},
        {}, {}
      },
      {
	{_min.x, (_min.y + _max.y) / 2, (_min.z + _max.z) / 2},
	{(_max.x + _min.x) / 2, _max.y, _max.z},
	{}, {}
      },
      {
        {_min.x, _min.y, (_min.z + _max.z) / 2},
	{(_max.x + _min.x) / 2, (_min.y + _max.y) / 2, _max.z},
        {}, {}
      },
      {
        {_min.x, (_min.y + _max.y) / 2, _min.z},
	{(_max.x + _min.x) / 2, _max.y, (_min.z + _max.z) / 2},
        {}, {}
      },
      {
        {_min.x, _min.y, _min.z},
        {(_max.x + _min.x) / 2, (_min.y + _max.y) / 2, (_min.z + _max.z) / 2},
        {}, {}
      }
    };

  for (int i = 0; i < 8; ++i)
    {
      for (Element* &element : _elements)
	{
	  const gle::Vector3<GLfloat>& min = element->getMinPoint();
	  const gle::Vector3<GLfloat>& max = element->getMaxPoint();
	  const gle::Vector3<GLfloat>& center = element->getCenter();
          if (center.x <= subdivisions[i].max.x && center.x >= subdivisions[i].min.x &&
              center.y <= subdivisions[i].max.y && center.y >= subdivisions[i].min.y &&
              center.z <= subdivisions[i].max.z && center.z >= subdivisions[i].min.z)
            subdivisions[i].elements.push_back(element);
	  else if (min.x <= subdivisions[i].max.x && max.x >= subdivisions[i].min.x &&
		   min.y <= subdivisions[i].max.y && max.y >= subdivisions[i].min.y &&
		   min.z <= subdivisions[i].max.z && max.z >= subdivisions[i].min.z)
	    subdivisions[i].partialsElements.push_back(element);
	}
    }
  std::vector<std::thread*> _threads;
  _leaf = true;
  for (int i = 0; i < 8; ++i)
    {
      bool doThread = false;
      if (subdivisions[i].elements.size() > 0)
	_children[i] = new gle::Octree::Node(subdivisions[i].min, subdivisions[i].max, subdivisions[i].elements,
					     subdivisions[i].partialsElements);
      if (_children[i])
	{
	  _leaf = false;
	  if (nbThreads && maxThreads)
	    {
	      if (*nbThreads < maxThreads)
		{
		  (*nbThreads)++;
		  doThread = true;
		}
	    }
	  if (!doThread)
	    _children[i]->splitNode(false, nbThreads, maxThreads, depth + 1);
	  else
	    _threads.push_back(new std::thread(split, this, true, nbThreads, maxThreads, depth + 1));
	}
    }
  for (std::thread* &_thread : _threads)
    {
      _thread->join();
      delete _thread;
    }
  if (thread)
    (*nbThreads)--;
}

void gle::Octree::_addDebugNode(gle::Octree::Node* node)
{
  if (node)
    {
      _debugNodes.push_back(node->getDebugMesh());
      for (int i = 0; i < 8; ++i)
	_addDebugNode(node->_children[i]);
    }
}

std::vector<gle::Mesh*>& gle::Octree::getDebugNodes()
{
  _debugNodes.clear();
  _addDebugNode(_root);
  return (_debugNodes);
}

std::list<gle::Octree::Element*> &gle::Octree::getElementsInFrustum(const gle::Matrix4<GLfloat>& projection,
								    const gle::Matrix4<GLfloat>& modelview)
{
  _elementsInFrustum.clear();
  gle::Matrix4<GLfloat> clip = projection * modelview;
  GLfloat t;
  
  _frustum[0][0] = clip[3] - clip[0];
  _frustum[0][1] = clip[7] - clip[4];
  _frustum[0][2] = clip[11] - clip[8];
  _frustum[0][3] = clip[15] - clip[12];
  t = sqrt(_frustum[0][0] * _frustum[0][0] + _frustum[0][1] * _frustum[0][1] + _frustum[0][2] * _frustum[0][2]);
  _frustum[0][0] /= t;
  _frustum[0][1] /= t;
  _frustum[0][2] /= t;
  _frustum[0][3] /= t;
  _frustum[1][0] = clip[3] + clip[0];
  _frustum[1][1] = clip[7] + clip[4];
  _frustum[1][2] = clip[11] + clip[8];
  _frustum[1][3] = clip[15] + clip[12];
  t = sqrt( _frustum[1][0] * _frustum[1][0] + _frustum[1][1] * _frustum[1][1] + _frustum[1][2] * _frustum[1][2] );
  _frustum[1][0] /= t;
  _frustum[1][1] /= t;
  _frustum[1][2] /= t;
  _frustum[1][3] /= t;
  _frustum[2][0] = clip[3] + clip[1];
  _frustum[2][1] = clip[7] + clip[5];
  _frustum[2][2] = clip[11] + clip[9];
  _frustum[2][3] = clip[15] + clip[13];
  t = sqrt( _frustum[2][0] * _frustum[2][0] + _frustum[2][1] * _frustum[2][1] + _frustum[2][2] * _frustum[2][2] );
  _frustum[2][0] /= t;
  _frustum[2][1] /= t;
  _frustum[2][2] /= t;
  _frustum[2][3] /= t;
  _frustum[3][0] = clip[3] - clip[1];
  _frustum[3][1] = clip[7] - clip[5];
  _frustum[3][2] = clip[11] - clip[9];
  _frustum[3][3] = clip[15] - clip[13];
  t = sqrt( _frustum[3][0] * _frustum[3][0] + _frustum[3][1] * _frustum[3][1] + _frustum[3][2] * _frustum[3][2] );
  _frustum[3][0] /= t;
  _frustum[3][1] /= t;
  _frustum[3][2] /= t;
  _frustum[3][3] /= t;
  _frustum[4][0] = clip[3] - clip[2];
  _frustum[4][1] = clip[7] - clip[6];
  _frustum[4][2] = clip[11] - clip[10];
  _frustum[4][3] = clip[15] - clip[14];
  t = sqrt( _frustum[4][0] * _frustum[4][0] + _frustum[4][1] * _frustum[4][1] + _frustum[4][2] * _frustum[4][2] );
  _frustum[4][0] /= t;
  _frustum[4][1] /= t;
  _frustum[4][2] /= t;
  _frustum[4][3] /= t;
  _frustum[5][0] = clip[3] + clip[2];
  _frustum[5][1] = clip[7] + clip[6];
  _frustum[5][2] = clip[11] + clip[10];
  _frustum[5][3] = clip[15] + clip[14];
  t = sqrt( _frustum[5][0] * _frustum[5][0] + _frustum[5][1] * _frustum[5][1] + _frustum[5][2] * _frustum[5][2] );
  _frustum[5][0] /= t;
  _frustum[5][1] /= t;
  _frustum[5][2] /= t;
  _frustum[5][3] /= t;
  _alreadyDone.clear();
  _root->addToFrustum(_frustum, _elementsInFrustum, &_alreadyDone);
  return (_elementsInFrustum);
}

void gle::Octree::Node::addToFrustum(const GLfloat frustum[6][4],
				     std::list<Element*>& elementsInFrustum,
				     std::map<Element*, bool>* alreadyDone)
{
  for(int i = 0; i < 6; i++)
    {
      if (frustum[i][0] * _min.x + frustum[i][1] * _min.y + frustum[i][2] * _min.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _max.x + frustum[i][1] * _min.y + frustum[i][2] * _min.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _min.x + frustum[i][1] * _max.y + frustum[i][2] * _min.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _max.x + frustum[i][1] * _max.y + frustum[i][2] * _min.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _min.x + frustum[i][1] * _min.y + frustum[i][2] * _max.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _max.x + frustum[i][1] * _min.y + frustum[i][2] * _max.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _min.x + frustum[i][1] * _max.y + frustum[i][2] * _max.z + frustum[i][3] > 0)
	continue;
      if (frustum[i][0] * _max.x + frustum[i][1] * _max.y + frustum[i][2] * _max.z + frustum[i][3] > 0)
	continue;
      return;
    }
  if (_leaf)
    {
      for (gle::Octree::Element* &element : _elements)
	{
	  if (element->isInFrustum(frustum) && (*alreadyDone)[element] == false)
	    {
	      (*alreadyDone)[element] = true;
	      elementsInFrustum.push_back(element);
	    }
	}
      for (gle::Octree::Element* &element : _partialsElements)
        {
          if (element->isInFrustum(frustum) && (*alreadyDone)[element] == false)
            {
	      (*alreadyDone)[element] = true;
	      elementsInFrustum.push_back(element);
	    }
	}
    }
  else
    {
      for (int i = 0; i < 8; ++i)
	if (_children[i])
	  _children[i]->addToFrustum(frustum, elementsInFrustum, alreadyDone);
      for (gle::Octree::Element* &element : _partialsElements)
        {
          if (element->isInFrustum(frustum) && (*alreadyDone)[element] == false)
            {
              (*alreadyDone)[element] = true;
	      elementsInFrustum.push_back(element);
	    }
	}
    }
}
