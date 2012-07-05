//
// PerspectiveCamera.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 12:53:42 2012 loick michard
// Last update Fri Jul  6 00:27:05 2012 loick michard
//

#include <PerspectiveCamera.hpp>
#include <Geometries.hpp>
#include <Renderer.hpp>
#include <Color.hpp>

gle::PerspectiveCamera::PerspectiveCamera(gle::Vector3<GLfloat>
					  const& position,
					  GLfloat fovy,
					  GLfloat aspect,
					  GLfloat near,
					  GLfloat far)
  : gle::Camera(position, position),  _fovy(fovy), _aspect(aspect),
    _near(near), _far(far), _hasChanged(false)
{
  _debugMesh[0] = NULL;
  _target += gle::Vector3<GLfloat>(0, 0, 1);
}

gle::PerspectiveCamera::PerspectiveCamera(gle::Vector3<GLfloat>
                                          const& position,
					  gle::Vector3<GLfloat>
                                          const& target,
                                          GLfloat fovy,
                                          GLfloat aspect,
                                          GLfloat near,
                                          GLfloat far)
  : gle::Camera(position, target), _fovy(fovy), _aspect(aspect),
    _near(near), _far(far), _hasChanged(false)
{
  _debugMesh[0] = NULL;
}

gle::PerspectiveCamera::~PerspectiveCamera()
{
  if (_debugMesh[0])
    {
      delete _debugMesh[0]->getMaterial();
      delete _debugMesh[0];
      delete _debugMesh[1]->getMaterial();
      delete _debugMesh[1];
    }
}


void gle::PerspectiveCamera::setFovy(GLfloat fovy)
{
  _fovy = fovy;
  _hasChanged = true;
  updateProjectionMatrix();
}

void gle::PerspectiveCamera::updateProjectionMatrix()
{
  _projectionMatrix = gle::Matrix4<GLfloat>::perspective(_fovy, _aspect,
							 _near, _far);
}

void gle::PerspectiveCamera::update()
{
  gle::Camera::update();
  if (_debugMesh[0])
    {
      Matrix4<GLfloat> tm = _transformationMatrix;
      Matrix3<GLfloat> nm = _normalMatrix;
      _debugMesh[0]->setMatrices(tm, nm);
      _debugMesh[1]->setMatrices(tm, nm);
    }
}

std::vector<gle::Scene::Node*>& gle::PerspectiveCamera::getDebugNodes(int mode)
{
  if (mode & Renderer::Camera)
    {
      if (_debugMesh[0] && _hasChanged)
	{
	  delete _debugMesh[0]->getMaterial();
	  delete _debugMesh[0];
	  delete _debugMesh[1]->getMaterial();
	  delete _debugMesh[1];
	  _hasChanged = false;
	  _debugMesh[0] = _debugMesh[1] = NULL;
	  _debugNodes.clear();
	}
      if (!_debugMesh[0])
	{
	  Material* mat = new Material();
	  mat->setAmbientColor(Colorf(1.0, 0, 0));
	  Material* mat2 = new Material();
	  mat2->setAmbientColor(Colorf(1.0, 0.5, 0));
	  _debugMesh[0] = new gle::Mesh(mat, true);
	  _debugMesh[1] = new gle::Mesh(mat2, true);
	  _debugMesh[0]->setRasterizationMode(Mesh::Line);
	  _debugMesh[1]->setRasterizationMode(Mesh::Line);
	  GLfloat yh = _near * tan(_fovy * M_PI / 180 / 2);
	  GLfloat xh = _near * tan(_aspect * _fovy * M_PI / 180 / 2);
	  GLfloat yh2 = _far * tan(_fovy * M_PI / 180 / 2);
	  GLfloat xh2 = _far * tan(_aspect * _fovy * M_PI / 180 / 2);
	  GLfloat vtx[] = {0, 0, 0, xh, yh, -_near, 0, 0, 0,
			   0, 0, 0, xh, -yh, -_near, 0, 0, 0,
			   0, 0, 0, -xh, -yh, -_near, 0, 0, 0,
			   0, 0, 0, -xh, yh, -_near, 0, 0, 0};
	  GLfloat vtx2[] = {xh, yh, -_near, xh2, yh2, -_far, xh, yh, -_near,
			    -xh, yh, -_near, -xh2, yh2, -_far, -xh, yh, -_near,
			    -xh, -yh, -_near, -xh2, -yh2, -_far, -xh, -yh, -_near,
			    xh, -yh, -_near, xh2, -yh2, -_far, xh, -yh, -_near,
			    xh, yh, -_near, xh, -yh, -_near, xh, yh, -_near,
			    xh, -yh, -_near, -xh, -yh, -_near, xh, -yh, -_near,
			    -xh, -yh, -_near, -xh, yh, -_near, -xh, -yh, -_near,
			    -xh, yh, -_near, xh, yh, -_near, -xh, yh, -_near,
			    xh2, yh2, -_far, -xh2, yh2, -_far, xh2, yh2, -_far,
			    -xh2, yh2, -_far, -xh2, -yh2, -_far, -xh2, yh2, -_far,
			    -xh2, -yh2, -_far, xh2, -yh2, -_far, -xh2, -yh2, -_far,
			    xh2, -yh2, -_far,  xh2, yh2, -_far, xh2, -yh2, -_far};
	  GLuint idx[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	  GLuint idx2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			   15, 16, 17, 18, 19, 20, 21, 22, 23,
			   24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
	  _debugMesh[0]->setVertexes(vtx, 36);
	  _debugMesh[0]->setIndexes(idx, 12);
	  _debugMesh[1]->setVertexes(vtx2, sizeof(vtx2) / sizeof(GLfloat));
	  _debugMesh[1]->setIndexes(idx2, sizeof(idx2) / sizeof(GLuint));
	  _debugNodes.push_back(_debugMesh[0]);
	  _debugNodes.push_back(_debugMesh[1]);
	  update();
	}
    }
  return (_debugNodes);
}
