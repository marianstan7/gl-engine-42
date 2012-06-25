//
// BoundingBox.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 16:41:28 2012 loick michard
// Last update Sat Jun 23 15:12:47 2012 loick michard
//

#include <BoundingBox.hpp>
#include <Geometries.hpp>

gle::BoundingBox::BoundingBox() : _debugMaterial(NULL), _debugMesh(NULL)
{

}

gle::BoundingBox::~BoundingBox()
{
  delete _debugMaterial;
  delete _debugMesh;
}


gle::BoundingVolume* gle::BoundingBox::duplicate() const
{
  gle::BoundingBox* box = new gle::BoundingBox();

  box->_debugMaterial = _debugMaterial;
  for (unsigned int i = 0; i < 8; ++i)
    {
      box->_points[i] = _points[i];
      box->_absolutePoints[i] = _absolutePoints[i];
    }
  if (_debugMesh)
    box->_debugMesh = new Mesh(*_debugMesh);
  else
    box->_debugMesh = NULL;
  box->_center = _center;
  return (box);
}

void gle::BoundingBox::setBestFit(const GLfloat* vertexes, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / gle::Mesh::VertexAttributeSizeCoords;

  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      if (i == 0 || vertexes[i * 3 + 0] < _min.x)
	_min.x = vertexes[i * 3 + 0];
      if (i == 0 || vertexes[i * 3 + 0] > _max.x)
	_max.x = vertexes[i * 3 + 0];
      if (i == 0 || vertexes[i * 3 + 1] < _min.y)
	_min.y = vertexes[i * 3 + 1];
      if (i == 0 || vertexes[i * 3 + 1] > _max.y)
	_max.y = vertexes[i * 3 + 1];
      if (i == 0 || vertexes[i * 3 + 2] < _min.z)
	_min.z = vertexes[i * 3 + 2];
      if (i == 0 || vertexes[i * 3 + 2] > _max.z)
	_max.z = vertexes[i * 3 + 2];
    }
  _center = (_max + _min);
  _center /= 2.0;
  _points[0] = Vector3<GLfloat> (_max.x, _max.y, _max.z);
  _points[1] = Vector3<GLfloat> (_min.x, _max.y, _max.z);
  _points[2] = Vector3<GLfloat> (_min.x, _min.y, _max.z);
  _points[3] = Vector3<GLfloat> (_max.x, _min.y, _max.z);
  _points[4] = Vector3<GLfloat> (_max.x, _max.y, _min.z);
  _points[5] = Vector3<GLfloat> (_min.x, _max.y, _min.z);
  _points[6] = Vector3<GLfloat> (_min.x, _min.y, _min.z);
  _points[7] = Vector3<GLfloat> (_max.x, _min.y, _min.z);
}

void gle::BoundingBox::setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes)
{
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      if (i == 0 || datas[offset + i * attributeSize + 0] < _min.x)
	_min.x = datas[offset + i * attributeSize + 0];
      if (i == 0 || datas[offset + i * attributeSize + 0] > _max.x)
	_max.x = datas[offset + i * attributeSize + 0];
      if (i == 0 || datas[offset + i * attributeSize + 1] < _min.y)
	_min.y = datas[offset + i * attributeSize + 1];
      if (i == 0 || datas[offset + i * attributeSize + 1] > _max.y)
	_max.y = datas[offset + i * attributeSize + 1];
      if (i == 0 || datas[offset + i * attributeSize + 2] < _min.z)
	_min.z = datas[offset + i * attributeSize + 2];
      if (i == 0 || datas[offset + i * attributeSize + 2] > _max.z)
	_max.z = datas[offset + i * attributeSize + 2];
    }
  _center = (_max + _min);
  _center /= 2.0;
  _points[0] = Vector3<GLfloat> (_max.x, _max.y, _max.z);
  _points[1] = Vector3<GLfloat> (_min.x, _max.y, _max.z);
  _points[2] = Vector3<GLfloat> (_min.x, _min.y, _max.z);
  _points[3] = Vector3<GLfloat> (_max.x, _min.y, _max.z);
  _points[4] = Vector3<GLfloat> (_max.x, _max.y, _min.z);
  _points[5] = Vector3<GLfloat> (_min.x, _max.y, _min.z);
  _points[6] = Vector3<GLfloat> (_min.x, _min.y, _min.z);
  _points[7] = Vector3<GLfloat> (_max.x, _min.y, _min.z);
}



gle::Mesh* gle::BoundingBox::getDebugMesh(gle::Mesh* mesh)
{
  if (!_debugMaterial)
    {
      _debugMaterial = new Material();
      _debugMaterial->setAmbientColor(Color<GLfloat>(1.0, 1.0, 0));
    }
  if (!_debugMesh)
    {
      _debugMesh = Geometries::Cuboid(_debugMaterial,
				      (_max.x - _min.x > 0) ? _max.x - _min.x : _min.x - _max.x,
				      (_max.y - _min.y > 0) ? _max.y - _min.y : _min.y - _max.y,
				      (_max.z - _min.z > 0) ? _max.z - _min.z : _min.z - _max.z, true);
      _debugMesh->setPosition(_center);
      _debugMesh->setRasterizationMode(Mesh::Line);

      Matrix4<GLfloat> mvMatrix = mesh->getTransformationMatrix();
      Matrix3<GLfloat> normalMatrix;

      mvMatrix.translate(_center);
      Matrix4<GLfloat> inverse(mvMatrix);
      inverse.inverse();
      normalMatrix = inverse;
      normalMatrix.transpose();
      _debugMesh->setMatrices(mvMatrix, normalMatrix);
    }
  return (_debugMesh);
}

void	gle::BoundingBox::update(Mesh* mesh)
{
  for (unsigned int i = 0; i < 8; ++i)
    {
      _absolutePoints[i] = _points[i];
      _absolutePoints[i] *= mesh->getTransformationMatrix();
      if (i == 0 || _absolutePoints[i].x < _min.x)
	_min.x = _absolutePoints[i].x;
      if (i == 0 || _absolutePoints[i].y < _min.y)
	_min.y = _absolutePoints[i].y;
      if (i == 0 || _absolutePoints[i].z < _min.z)
	_min.z = _absolutePoints[i].z;
      if (i == 0 || _absolutePoints[i].x > _max.x)
        _max.x = _absolutePoints[i].x;
      if (i == 0 || _absolutePoints[i].y > _max.y)
        _max.y = _absolutePoints[i].y;
      if (i == 0 || _absolutePoints[i].z > _max.z)
        _max.z = _absolutePoints[i].z;
    }
  _absoluteCenter = (_max + _min);
  _absoluteCenter /= 2.0;
  if (_debugMesh)
    {
      Matrix4<GLfloat> mvMatrix = mesh->getTransformationMatrix();
      Matrix3<GLfloat> normalMatrix;

      mvMatrix.translate(_center);
      Matrix4<GLfloat> inverse(mvMatrix);
      inverse.inverse();
      normalMatrix = inverse;
      normalMatrix.transpose();
      _debugMesh->setMatrices(mvMatrix, normalMatrix);
      _debugMesh->setRasterizationMode(gle::Mesh::Line);
    }
}

const gle::Vector3<float>& gle::BoundingBox::getMaxPoint() const
{
  return (_max);
}

const gle::Vector3<float>& gle::BoundingBox::getMinPoint() const
{
  return (_min);
}

const gle::Vector3<float>& gle::BoundingBox::getCenter() const
{
  return (_absoluteCenter);
}

bool gle::BoundingBox::isInFrustum(const GLfloat frustum[6][4]) const
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
      return false;
    }
  return true;
}
