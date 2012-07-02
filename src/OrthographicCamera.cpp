//
// OrthographicCamera.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 12:53:42 2012 loick michard
// Last update Tue Jun 26 09:46:19 2012 gael jochaud-du-plessix
//

#include <OrthographicCamera.hpp>

gle::OrthographicCamera::OrthographicCamera(gle::Vector3<GLfloat> const& position,
					    GLfloat near, GLfloat far,
					    GLfloat left, GLfloat right,
					    GLfloat bottom, GLfloat top)
  : gle::Camera(position, position),
    _near(near), _far(far), _left(left),
    _right(right), _bottom(bottom), _top(top)
{
  _target += gle::Vector3<GLfloat>(0, 0, 1);
  this->updateMatrix();
}

gle::OrthographicCamera::OrthographicCamera(gle::Vector3<GLfloat> const& position,
					    gle::Vector3<GLfloat> const& target,
					    GLfloat near, GLfloat far,
					    GLfloat left, GLfloat right,
					    GLfloat bottom, GLfloat top)
  : gle::Camera(position, target),
    _near(near), _far(far), _left(left),
    _right(right), _bottom(bottom), _top(top)
{
  this->updateMatrix();
}

void gle::OrthographicCamera::updateProjectionMatrix()
{
  _projectionMatrix = gle::Matrix4<GLfloat>::orthographic(_near, _far, _left, _right, _bottom, _top);
}
