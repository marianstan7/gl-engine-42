//
// DirectionalLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Tue Jun 26 11:20:42 2012 gael jochaud-du-plessix
//

#include <DirectionalLight.hpp>

gle::DirectionalLight::DirectionalLight(Vector3<GLfloat> const& direction,
					Color<GLfloat> const& color)
  : gle::Light(gle::Light::DIRECTIONAL), _direction(direction)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
}

gle::DirectionalLight::~DirectionalLight()
{

}

void gle::DirectionalLight::setDirection(Vector3<GLfloat> const& direction)
{
  _direction = direction;
}

void gle::DirectionalLight::setColor(Color<GLfloat> const& color)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
}

gle::Vector3<GLfloat>& gle::DirectionalLight::getDirection()
{
  return (_direction);
}

GLfloat* gle::DirectionalLight::getColor()
{
  return (_color);
}
