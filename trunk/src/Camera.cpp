//
// Camera.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 11:16:08 2012 loick michard
// Last update Tue Jun 26 09:47:13 2012 gael jochaud-du-plessix
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

const gle::Matrix4<GLfloat>& gle::Camera::getProjectionMatrix() const
{
  return (_projectionMatrix);
}

void gle::Camera::update()
{
  this->updateProjectionMatrix();
}
