//
// Mesh.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Mon Feb 20 18:25:23 2012 loick michard
// Last update Thu May 31 20:01:35 2012 gael jochaud-du-plessix
//

#include <Mesh.hpp>
#include <BoundingBox.hpp>
#include <BoundingBox.hpp>

gle::Mesh::Mesh(Material* material)
  : gle::Scene::Node(gle::Scene::Node::Mesh),
    _primitiveType(Triangles),
    _rasterizationMode(Fill),
    _pointSize(1.0),
    _material(material),
    _indexes(NULL),
    _attributes(NULL),
    _nbIndexes(0),
    _nbVertexes(0),
    _boundingVolume(NULL)
{
  _indexes = new gle::Buffer<GLuint>(gle::Buffer<GLuint>::ElementArray,
				     gle::Buffer<GLuint>::StaticDraw);
}

gle::Mesh::Mesh(gle::Mesh const & other)
  : gle::Scene::Node(other),
    _primitiveType(other._primitiveType),
    _rasterizationMode(other._rasterizationMode),
    _pointSize(other._pointSize),
    _material(other._material),
    _indexes(NULL),
    _attributes(NULL),
    _nbIndexes(other._nbIndexes),
    _nbVertexes(other._nbVertexes)
{
  if (other._boundingVolume)
    _boundingVolume = other._boundingVolume->duplicate();
  else
    _boundingVolume = NULL;
  static int max = 0, nb = 0;
  max += _nbVertexes;
  nb++;
  _indexes =  new gle::Buffer<GLuint>(*other._indexes);
  _attributes = other._attributes;
  if (_attributes)
    _attributes->retain();
}

gle::Mesh::~Mesh()
{
  if (_indexes)
    delete _indexes;
  if (_attributes)
    _attributes->release();
  if (_boundingVolume)
    delete _boundingVolume;
}

void gle::Mesh::setPrimitiveType(PrimitiveType type)
{
  _primitiveType = type;
}

gle::Mesh::PrimitiveType gle::Mesh::getPrimitiveType() const
{
  return (_primitiveType);
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

GLfloat gle::Mesh::getPointSize() const
{
  return (_pointSize);
}

void gle::Mesh::setVertexAttributes(const GLfloat* attributes, GLsizeiptr nbVertexes)
{
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(attributes, nbVertexes * VertexAttributesSize);
  else
    _attributes = MeshBufferManager::getInstance()
      .resize(_attributes, nbVertexes * VertexAttributesSize, attributes);
  _nbVertexes = nbVertexes;
}

void gle::Mesh::setVertexes(const GLfloat* vertexes, GLsizeiptr size, bool boundingVolume)
{
  GLsizeiptr nbVertexes = size / VertexAttributeSizeCoords;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLuint j = 0; j < VertexAttributeSizeCoords; ++j)
      attributes[i * VertexAttributesSize + j] =
	vertexes[i * VertexAttributeSizeCoords + j];
  attributes[0] = vertexes[0];
  _attributes->unmap();
  if (boundingVolume)
    {
      if (_boundingVolume)
	delete _boundingVolume;
      _boundingVolume = new BoundingBox();
      _boundingVolume->setBestFit(vertexes, size);
    }
}

void gle::Mesh::setNormals(const GLfloat* normals, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / VertexAttributeSizeNormal;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLuint j = 0; j < VertexAttributeSizeNormal; ++j)
      attributes[i * VertexAttributesSize + VertexAttributeSizeCoords + j] =
	normals[i * VertexAttributeSizeNormal + j];
  _attributes->unmap();
}

void gle::Mesh::setTextureCoords(const GLfloat* textureCoords, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / VertexAttributeSizeTextureCoords;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLuint j = 0; j < VertexAttributeSizeTextureCoords; ++j)
      attributes[i * VertexAttributesSize + VertexAttributeSizeCoords +
		 VertexAttributeSizeNormal + j] =
	textureCoords[i * VertexAttributeSizeTextureCoords + j];
  _attributes->unmap();
}

void gle::Mesh::setIndexes(const GLuint* indexes, GLsizeiptr size)
{
  _nbIndexes = size;
  _indexes->resize(size, indexes);
}

void gle::Mesh::setVertexes(gle::Array<GLfloat> const &vertexes, bool boundingVolume)
{
  GLsizeiptr nbVertexes = vertexes.size() / VertexAttributeSizeCoords;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLsizeiptr j = 0; j < VertexAttributeSizeCoords; ++j)
      attributes[i * VertexAttributesSize + j] =
	vertexes[(GLuint)(i * VertexAttributeSizeCoords + j)];
  _attributes->unmap();
  if (boundingVolume)
    {
      if (_boundingVolume)
	delete _boundingVolume;
      _boundingVolume = new BoundingBox();
      _boundingVolume->setBestFit(vertexes, vertexes.size());
    }
}

void gle::Mesh::setNormals(gle::Array<GLfloat> const &normals)
{
  GLsizeiptr nbVertexes = normals.size() / VertexAttributeSizeNormal;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLuint j = 0; j < VertexAttributeSizeNormal; ++j)
      attributes[i * VertexAttributesSize + VertexAttributeSizeCoords + j] =
	normals[(GLuint)(i * VertexAttributeSizeNormal + j)];
  _attributes->unmap();
}

void gle::Mesh::setTextureCoords(gle::Array<GLfloat> const &textureCoords)
{
  GLsizeiptr nbVertexes = textureCoords.size() / VertexAttributeSizeTextureCoords;

  _nbVertexes = nbVertexes;
  if (!_attributes)
    _attributes = MeshBufferManager::getInstance()
      .store(NULL, nbVertexes * VertexAttributesSize);
  GLfloat* attributes = _attributes->map();
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    for (GLuint j = 0; j < VertexAttributeSizeTextureCoords; ++j)
      attributes[i * VertexAttributesSize + VertexAttributeSizeCoords +
		 VertexAttributeSizeNormal + j] =
	textureCoords[(GLuint)(i * VertexAttributeSizeTextureCoords + j)];
  _attributes->unmap();
}

void gle::Mesh::setIndexes(gle::Array<GLuint> const &indexes)
{
  _nbIndexes = indexes.size();
  _indexes->resize(indexes.size(), (GLuint const *)indexes);
}

void gle::Mesh::setMaterial(gle::Material* material)
{
  _material = material;
}

gle::Material* gle::Mesh::getMaterial() const
{
  return (_material);
}

gle::Buffer<GLuint> * gle::Mesh::getIndexesBuffer() const
{
  return (_indexes);
}

gle::MeshBufferManager::Chunk* gle::Mesh::getAttributes() const
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

gle::BoundingVolume* gle::Mesh::getBoundingVolume() const
{
  return (_boundingVolume);
}

const gle::Vector3<GLfloat>& gle::Mesh::getMaxPoint() const
{
  if (_boundingVolume)
    return (_boundingVolume->getMaxPoint());
  return (_position);
}

const gle::Vector3<GLfloat>& gle::Mesh::getMinPoint() const
{
  if (_boundingVolume)
    return (_boundingVolume->getMinPoint());
  return (_position);
}

const gle::Vector3<GLfloat>& gle::Mesh::getCenter() const
{
  if (_boundingVolume)
    return (_boundingVolume->getCenter());
  return (_position);
}

bool gle::Mesh::isInFrustum(const GLfloat frustum[6][4]) const
{
  if (_boundingVolume)
    return (_boundingVolume->isInFrustum(frustum));
  return (true);
}

gle::Scene::Node* gle::Mesh::duplicate() const
{
  return (new Mesh(*this));
}

void gle::Mesh::update()
{
  if (_boundingVolume)
    _boundingVolume->update(this);
}
