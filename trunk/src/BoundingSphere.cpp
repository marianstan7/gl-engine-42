//
// BoundingSphere.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed May  2 16:41:28 2012 loick michard
// Last update Thu Jun 21 20:19:53 2012 loick michard
//

#include <BoundingSphere.hpp>
#include <Geometries.hpp>

gle::BoundingSphere::BoundingSphere() :
  _center(0, 0, 0), _absoluteRadius(0),
  _material(), _mesh(NULL)
{
  _material.setDiffuseLightEnabled(true);
  _material.setSpecularLightEnabled(true);
  _material.setDiffuseColor(gle::Color<GLfloat>(255, 0, 0));
}

gle::BoundingSphere::~BoundingSphere()
{
  delete _mesh;
}


gle::BoundingVolume* gle::BoundingSphere::duplicate() const
{
  gle::BoundingSphere* sphere = new gle::BoundingSphere();

  sphere->_material = _material;
  sphere->_center = _center;
  sphere->_absoluteCenter = _absoluteCenter;
  sphere->_radius = _radius;
  sphere->_absoluteRadius = _absoluteRadius;
  if (_mesh)
    sphere->_mesh = new Mesh(*_mesh);
  else
    sphere->_mesh = NULL;
  sphere->_min = _min;
  sphere->_max = _max;
  return (sphere);
}

void gle::BoundingSphere::setBestFit(const GLfloat* vertexes, GLsizeiptr size)
{
  GLsizeiptr nbVertexes = size / gle::Mesh::VertexAttributeSizeCoords;
  
   _center.x = 0;
  _center.y = 0;
  _center.z = 0;
  _radius = -1;
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      _center.x += vertexes[i * 3] / nbVertexes;
      _center.y += vertexes[i * 3 + 1] / nbVertexes;
      _center.z += vertexes[i * 3 + 2] / nbVertexes;
    }
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      GLfloat distance = (_center.x - vertexes[i * 3]) * (_center.x - vertexes[i * 3]) +
	(_center.y - vertexes[i * 3 + 1]) * (_center.y - vertexes[i * 3 + 1]) +
	(_center.z - vertexes[i * 3 + 2]) * (_center.z - vertexes[i * 3 + 2]);
      if (_radius == -1 || distance > _radius)
	_radius = distance;
    }
  _radius = sqrt(_radius);
  /*  if (!_mesh)
    _mesh = gle::Geometries::Sphere(&_material, _radius, 15, 15, false); 
  _mesh->setPosition(_center);
  _mesh->setRasterizationMode(gle::Mesh::Line);*/
  _min = _center;
  _min -= _radius;
  _max = _center;
  _max += _radius;
}

void gle::BoundingSphere::setBestFit(const GLfloat* datas, GLsizeiptr offset, GLsizeiptr attributeSize, GLsizeiptr nbVertexes)
{
  _center.x = 0;
  _center.y = 0;
  _center.z = 0;
  _radius = -1;
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      _center.x += datas[offset + i * attributeSize] / nbVertexes;
      _center.y += datas[offset + i * attributeSize + 1] / nbVertexes;
      _center.z += datas[offset + i * attributeSize + 2] / nbVertexes;
    }
  for (GLsizeiptr i = 0; i < nbVertexes; ++i)
    {
      GLfloat distance = (_center.x - datas[offset + i * attributeSize]) * (_center.x - datas[offset + i * attributeSize]) +
	(_center.y - datas[offset + i * attributeSize + 1]) * (_center.y - datas[offset + i * attributeSize + 1]) +
	(_center.z - datas[offset + i * attributeSize + 2]) * (_center.z - datas[offset + i * attributeSize + 2]);
      if (_radius == -1 || distance > _radius)
	_radius = distance;
    }
  _radius = sqrt(_radius);
  /*  if (!_mesh)
    _mesh = gle::Geometries::Sphere(&_material, _radius, 15, 15, false); 
  _mesh->setPosition(_center);
  _mesh->setRasterizationMode(gle::Mesh::Line);*/
  _min = _center;
  _min -= _radius;
  _max = _center;
  _max += _radius;
}

gle::Mesh* gle::BoundingSphere::getMesh() const
{
  return (_mesh);
}

void	gle::BoundingSphere::update(Mesh* mesh)
{
  Vector3<GLfloat> point = _center;
  point.x += _radius;
  _absoluteCenter = _center;
  _absoluteCenter *= mesh->getTransformationMatrix();
  point *= mesh->getTransformationMatrix();
  _absoluteRadius = sqrt((point.x - _absoluteCenter.x) * (point.x - _absoluteCenter.x) +
		 (point.y - _absoluteCenter.y) * (point.y - _absoluteCenter.y) +
		 (point.z - _absoluteCenter.z) * (point.z - _absoluteCenter.z));
  if (_mesh)
    {
      Matrix4<GLfloat> mvMatrix = mesh->getTransformationMatrix();
      Matrix3<GLfloat> normalMatrix;

      mvMatrix.translate(_center);
      Matrix4<GLfloat> inverse(mvMatrix);
      inverse.inverse();
      normalMatrix = inverse;
      normalMatrix.transpose();
      _mesh->setMatrices(mvMatrix, normalMatrix);
      _mesh->setRasterizationMode(gle::Mesh::Line);
    }
  _min = _absoluteCenter;
  _min -= _absoluteRadius;
  _max = _absoluteCenter;
  _max += _absoluteRadius;
}

const gle::Vector3<float>& gle::BoundingSphere::getMaxPoint() const
{
  return (_max);
}

const gle::Vector3<float>& gle::BoundingSphere::getMinPoint() const
{
  return (_min);
}

const gle::Vector3<float>& gle::BoundingSphere::getCenter() const
{
  return (_absoluteCenter);
}

bool gle::BoundingSphere::isInFrustum(const GLfloat frustum[6][4]) const
{
  (void)frustum;
#warning Todo: BoundingSphere::isInFrustum
  return (false);
}
