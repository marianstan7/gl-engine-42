//
// Camera.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 11:16:08 2012 loick michard
// Last update Wed Feb 22 18:17:39 2012 loick michard
//

#include <Camera.hpp>

gle::Camera::Camera(gle::Vector3<GLfloat> const& position,
		    gle::Vector3<GLfloat> const& target)
  : _position(position), _target(target)
{

}

gle::Camera::~Camera()
{

}

void gle::Camera::setPosition(gle::Vector3<GLfloat> const& position)
{
  _position = position;
  this->updateMatrix();
}

void gle::Camera::setTarget(gle::Vector3<GLfloat> const& target)
{
  _target = target;
  this->updateMatrix();
}

void gle::Camera::setRotation(gle::Vector3<GLfloat> const& axis, GLfloat angle)
{
  _rotation.setRotation(axis, angle);
  this->updateMatrix();
}

void gle::Camera::translate(gle::Vector3<GLfloat> const& vec)
{
  _position += vec;
  _target += vec;
  this->updateMatrix();
}

gle::Vector3<GLfloat> const& gle::Camera::getPosition() const
{
  return (_position);
}

gle::Vector3<GLfloat> const& gle::Camera::getTarget() const
{
  return (_target);
}

gle::Matrix4<GLfloat>& gle::Camera::getMatrix()
{
  return (_matrix);
}
