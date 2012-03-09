//
// Mesh.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 18:25:23 2012 loick michard
// Last update Fri Mar  9 22:45:53 2012 gael jochaud-du-plessix
//

#include <algorithm>
#include <Mesh.hpp>

gle::Mesh::Mesh(Material* material,
		const GLfloat* vertexes, GLsizeiptr nbVertexes,
		const GLfloat* normals, GLsizeiptr nbNormals,
		const GLuint* indexes, GLsizeiptr nbIndexes)
  : _name(""),
    _type(Triangles),
    _pointSize(1.0),
    _material(material),
    _vertexes(NULL),
    _normals(NULL),
    _textureCoords(NULL),
    _indexes(NULL),
    _nbIndexes(nbIndexes),
    _position(0, 0, 0), _hasTarget(false), _parentMatrix(NULL)
{
  if (vertexes)
    _vertexes = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					 gle::Buffer<GLfloat>::StaticDraw,
					 nbVertexes, vertexes);
  if (normals)
    _normals = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					gle::Buffer<GLfloat>::StaticDraw,
					nbNormals, normals);
  if (indexes)
    _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
				       gle::Buffer<GLuint>::StaticDraw,
				       nbIndexes, indexes);
}

gle::Mesh::Mesh(Material* material,
		gle::Array<GLfloat> const * vertexes,
		gle::Array<GLfloat> const * normals,
		gle::Array<GLuint> const * indexes)
  : _name(""),
    _type(Triangles),
    _pointSize(1.0),
    _material(material),
    _vertexes(NULL),
    _normals(NULL),
    _textureCoords(NULL),
    _indexes(NULL),
    _nbIndexes(0),
    _position(0, 0, 0), _hasTarget(false), _parentMatrix(NULL)
{
  if (vertexes)
    _vertexes = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					 gle::Buffer<GLfloat>::StaticDraw,
					 vertexes->size(),
					 (GLfloat const *)(*vertexes));
  if (normals)
    _normals = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					gle::Buffer<GLfloat>::StaticDraw,
					normals->size(),
					(GLfloat const *)(*normals));
  if (indexes)
    {
      _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
					 gle::Buffer<GLuint>::StaticDraw,
					 indexes->size(),
					 (GLuint const *)(*indexes));
      _nbIndexes = indexes->size();
    }
}

gle::Mesh::~Mesh()
{
  if (_vertexes)
    delete _vertexes;
  if (_normals)
    delete _normals;
  if (_indexes)
    delete (_indexes);
}

void gle::Mesh::addChild(gle::Mesh* child)
{
  if (find(_children.begin(), _children.end(), child) == _children.end())
    {
      _children.push_back(child);
      child->setParentMatrix(&_mvMatrix);
    }
}

std::vector<gle::Mesh*> & gle::Mesh::getChildren()
{
  return (_children);
}

void gle::Mesh::getChildrenByName(std::string const & name,
				  std::vector<gle::Mesh*> & vector)
{
  std::vector<gle::Mesh*> & childs = getChildren();
  for (std::vector<gle::Mesh*>::iterator it = childs.begin();
       it != childs.end(); ++it)
    {
      if ((*it)->getName() == name)
	vector.push_back(*it);
      (*it)->getChildrenByName(name, vector);
    }
}

gle::Mesh* gle::Mesh::getChildByName(std::string const & name)
{
  std::vector<gle::Mesh*> & childs = getChildren();
  for (std::vector<gle::Mesh*>::iterator it = childs.begin();
       it != childs.end(); ++it)
    {
      if ((*it)->getName() == name)
	return (*it);
    }
  return (NULL);
}

void gle::Mesh::setName(std::string const & name)
{
  _name = name;
}

std::string const & gle::Mesh::getName()
{
  return (_name);
}

void gle::Mesh::setType(Type type)
{
  _type = type;
}

gle::Mesh::Type gle::Mesh::getType()
{
  return (_type);
}

void gle::Mesh::setPointSize(GLfloat v)
{
  _pointSize = v;
}

GLfloat gle::Mesh::getPointSize()
{
  return (_pointSize);
}

void gle::Mesh::setParentMatrix(gle::Matrix4<GLfloat>* parentMatrix)
{
  _parentMatrix = parentMatrix;
  this->updateMatrix();
}

void gle::Mesh::setRotation(Vector3<GLfloat> const& axis, GLfloat angle)
{
  _rotation.setRotation(axis, angle);
  this->updateMatrix();
}

void gle::Mesh::setRotationTarget(const gle::Vector3<GLfloat>& target)
{
  _target = target;
  _hasTarget = true;
  this->updateMatrix();
}

void gle::Mesh::setScale(GLfloat x, GLfloat y, GLfloat z)
{
  _scaleMatrix = gle::Matrix4<GLfloat>::scale(x, y, z);
  this->updateMatrix();
}

void gle::Mesh::setScale(GLfloat scale)
{
  this->setScale(scale, scale, scale);
}

void gle::Mesh::setVertexes(const GLfloat* vertexes, GLsizeiptr nbVertexes)
{
  if (_vertexes)
    _vertexes->resize(nbVertexes, vertexes);
  else
    _vertexes = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					 gle::Buffer<GLfloat>::StaticDraw,
					 nbVertexes, vertexes);
}

void gle::Mesh::setNormals(const GLfloat* normals, GLsizeiptr nbNormals)
{
  if (_normals)
    _normals->resize(nbNormals, normals);
  else
    _normals = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					gle::Buffer<GLfloat>::StaticDraw,
					nbNormals, normals);
}

void gle::Mesh::setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size)
{
  if (_textureCoords)
    {
      if (textureCoords == NULL)
	{
	  delete _textureCoords;
	  _textureCoords = NULL;
	}
      else
	_textureCoords->resize(size, textureCoords);
    }
  else
    _textureCoords =
      new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
                               gle::Buffer<GLfloat>::StaticDraw,
                               size, textureCoords);
}

void gle::Mesh::setIndexes(const GLuint* indexes, GLsizeiptr nbIndexes)
{
  _nbIndexes = nbIndexes;
  if (_indexes)
    _indexes->resize(nbIndexes, indexes);
  else
    _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
					gle::Buffer<GLuint>::StaticDraw,
					nbIndexes, indexes);
}

void gle::Mesh::setVertexes(gle::Array<GLfloat> const &vertexes)
{
  if (_vertexes)
    _vertexes->resize(vertexes.size(), (GLfloat const *)vertexes);
  else
    _vertexes = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					 gle::Buffer<GLfloat>::StaticDraw,
					 vertexes.size(),
					 (GLfloat const *)vertexes);
}

void gle::Mesh::setNormals(gle::Array<GLfloat> const &normals)
{
  if (_normals)
    _normals->resize(normals.size(), (GLfloat const *)normals);
  else
    _normals = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					gle::Buffer<GLfloat>::StaticDraw,
					normals.size(),
					(GLfloat const *)normals);
}

void gle::Mesh::setTextureCoords(gle::Array<GLfloat> const &textureCoords)
{
  if (_textureCoords)
    _textureCoords->resize(textureCoords.size(),
			   (GLfloat const *)textureCoords);
  else
    _textureCoords =
      new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
			       gle::Buffer<GLfloat>::StaticDraw,
			       textureCoords.size(),
			       (GLfloat const *)textureCoords);
}

void gle::Mesh::setIndexes(gle::Array<GLuint> const &indexes)
{
  _nbIndexes = indexes.size();
  if (_indexes)
    _indexes->resize(indexes.size(), (GLuint const *)indexes);
  else
    _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
				       gle::Buffer<GLuint>::StaticDraw,
				       indexes.size(),
				       (GLuint const *)indexes);
}

void gle::Mesh::updateMatrix()
{
  if (_parentMatrix)
    _mvMatrix = *_parentMatrix;
  else
    _mvMatrix.identity();
  _mvMatrix.translate(_position);
  _mvMatrix *= _rotation.getMatrix();
  _mvMatrix *= _scaleMatrix;
  if (_children.size())
    {
      std::vector<Mesh*>::iterator b = _children.begin(); 
      std::vector<Mesh*>::iterator e = _children.end(); 
      while (b != e)
	{
	  (*b)->updateMatrix();
	  ++b;
	}
    }
  Matrix4<GLfloat> inverse(_mvMatrix);
  inverse.inverse();
  _normalMatrix = inverse;
  _normalMatrix.transpose();
}

void gle::Mesh::translate(gle::Vector3<GLfloat> const& vec)
{
  _position += vec;
  this->updateMatrix();
}

void gle::Mesh::setPosition(gle::Vector3<GLfloat> const& vec)
{
  _position = vec;
  this->updateMatrix();
}

gle::Matrix4<GLfloat>& gle::Mesh::getMatrix()
{
  return (_mvMatrix);
}

void gle::Mesh::setMaterial(gle::Material* material)
{
  _material = material;
}

gle::Matrix3<GLfloat>& gle::Mesh::getNormalMatrix()
{
  return (_normalMatrix);
}

gle::Material* gle::Mesh::getMaterial()
{
  return (_material);
}

gle::Buffer<GLfloat> * gle::Mesh::getVertexesBuffer()
{
  return (_vertexes);
}

gle::Buffer<GLfloat> * gle::Mesh::getNormalsBuffer()
{
  return (_normals);
}

gle::Buffer<GLfloat> * gle::Mesh::getTextureCoordsBuffer()
{
  return (_textureCoords);
}

gle::Buffer<GLuint> * gle::Mesh::getIndexesBuffer()
{
  return (_indexes);
}

GLsizeiptr gle::Mesh::getNbIndexes()
{
  return (_nbIndexes);
}
