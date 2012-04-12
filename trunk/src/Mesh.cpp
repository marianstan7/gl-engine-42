//
// Mesh.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 18:25:23 2012 loick michard
// Last update Thu Apr 12 18:09:03 2012 loick michard
//

#include <algorithm>
#include <Mesh.hpp>

gle::Mesh::Mesh(Material* material)
  : _name(""),
    _type(Triangles),
    _rasterizationMode(Fill),
    _pointSize(1.0),
    _material(material),
    _indexes(NULL),
    _attributes(NULL),
    _uniforms(NULL),
    _nbIndexes(0),
    _nbVertexes(0),
    _position(0, 0, 0),
    _hasTarget(false),
    _rotation(),
    _scaleMatrix(),
    _mvMatrix(),
    _normalMatrix(),
    _parentMatrix(NULL)
{
  _attributes = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::VertexArray,
					 gle::Buffer<GLfloat>::StaticDraw);
  _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
				     gle::Buffer<GLuint>::StaticDraw);
  _uniforms = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::UniformArray,
				       gle::Buffer<GLfloat>::StaticDraw);
  _uniforms->resize(27);
}

gle::Mesh::Mesh(gle::Mesh const & other)
  : _name(other._name),
    _type(other._type),
    _rasterizationMode(other._rasterizationMode),
    _pointSize(other._pointSize),
    _material(other._material),
    _indexes(NULL),
    _attributes(NULL),
    _uniforms(NULL),
    _nbIndexes(other._nbIndexes),
    _nbVertexes(other._nbVertexes),
    _position(other._position),
    _target(other._target),
    _hasTarget(other._hasTarget),
    _rotation(other._rotation),
    _scaleMatrix(other._scaleMatrix),
    _mvMatrix(other._mvMatrix),
    _normalMatrix(other._mvMatrix),
    _parentMatrix(other._parentMatrix)
{
  _indexes =  new gle::Buffer<GLuint>(*other._indexes);
  _attributes = new gle::Buffer<GLfloat>(*other._attributes);
  _uniforms = new gle::Buffer<GLfloat>(*other._uniforms);
  _uniforms->resize(27);
  std::vector<gle::Mesh*> children = other._children;
  for (std::vector<gle::Mesh*>::iterator it = children.begin(),
	 end = children.end(); it != end; ++it)
    addChild(new gle::Mesh(*(*it)));
}

gle::Mesh::~Mesh()
{
  if (_indexes)
    delete _indexes;
  if (_attributes)
    delete _attributes;
  if (_uniforms)
    delete _uniforms;
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

void gle::Mesh::setType(PrimitiveType type)
{
  _type = type;
}

gle::Mesh::PrimitiveType gle::Mesh::getType() const
{
  return (_type);
}

void gle::Mesh::setRasterizationMode(RasterizationMode rasterizationMode)
{
  _rasterizationMode = rasterizationMode;
}

gle::Mesh::RasterizationMode gle::Mesh::getRasterizationMode() const
{
  return (_rasterizationMode);
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

void gle::Mesh::setVertexes(const GLfloat* vertexes, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / VertexAttributeSizeCoords;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData(vertexes, 0, size);
}

void gle::Mesh::setNormals(const GLfloat* normals, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / VertexAttributeSizeNormal;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData(normals,
		       nbVertexes * VertexAttributeSizeCoords,
		       size);
}

void gle::Mesh::setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size
    / VertexAttributeSizeTextureCoords;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData(textureCoords,
		       nbVertexes * (VertexAttributeSizeCoords
				     + VertexAttributeSizeNormal),
		       size);
}

void gle::Mesh::setIndexes(const GLuint* indexes, GLsizeiptr size)
{
  _nbIndexes = size;
  _indexes->resize(size, indexes);
}

void gle::Mesh::setVertexes(gle::Array<GLfloat> const &vertexes)
{
  GLsizeiptr nbVertexes = vertexes.size() / VertexAttributeSizeCoords;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData((const GLfloat*)vertexes, 0,
		       nbVertexes * VertexAttributeSizeCoords);
}

void gle::Mesh::setNormals(gle::Array<GLfloat> const &normals)
{
  GLsizeiptr nbVertexes = normals.size() / VertexAttributeSizeNormal;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData((const GLfloat*)normals,
		       nbVertexes * VertexAttributeSizeCoords,
		       nbVertexes * VertexAttributeSizeNormal);
}

void gle::Mesh::setTextureCoords(gle::Array<GLfloat> const &textureCoords)
{
  GLsizeiptr nbVertexes = textureCoords.size()
    / VertexAttributeSizeTextureCoords;
  if (nbVertexes != _nbVertexes)
    {
      _nbVertexes = nbVertexes;
      _attributes->resize(VertexAttributesSize * nbVertexes);
    }
  _attributes->setData((const GLfloat*)textureCoords,
		       nbVertexes * (VertexAttributeSizeCoords
				     + VertexAttributeSizeNormal),
		       nbVertexes * VertexAttributeSizeTextureCoords);
}

void gle::Mesh::setIndexes(gle::Array<GLuint> const &indexes)
{
  _nbIndexes = indexes.size();
  _indexes->resize(indexes.size(), (GLuint const *)indexes);
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

gle::Buffer<GLuint> * gle::Mesh::getIndexesBuffer()
{
  return (_indexes);
}

gle::Buffer<GLfloat>* gle::Mesh::getAttributesBuffer()
{
  return (_attributes);
}

GLsizeiptr gle::Mesh::getNbIndexes() const
{
  return (_nbIndexes);
}

GLsizeiptr gle::Mesh::getNbVertexes() const
{
  return (_nbVertexes);
}

gle::Buffer<GLfloat>* gle::Mesh::getUniforms() const
{
  return (_uniforms);
}

