//
// Camera.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 11:16:08 2012 loick michard
// Last update Thu May 24 14:43:22 2012 loick michard
//

#include <Camera.hpp>

gle::Camera::Camera(gle::Vector3<GLfloat> const& position,
		    gle::Vector3<GLfloat> const& target)
  : gle::Scene::Node(gle::Scene::Node::Camera)
{
  _position = position;
  _target = target;
  _hasTarget = true;
}

gle::Camera::~Camera()
{

}
/*
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
*/
gle::Matrix4<GLfloat>& gle::Camera::getProjectionMatrix()
{
  return (_projectionMatrix);
}
/*
gle::Matrix4<GLfloat>& gle::Camera::getModelViewMatrix()
{
  return (_modelViewMatrix);
}
*/
