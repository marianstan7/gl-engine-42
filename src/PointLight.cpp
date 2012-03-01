//
// PointLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Tue Feb 28 16:21:23 2012 loick michard
//

#include <PointLight.hpp>

gle::PointLight::PointLight(Vector3<GLfloat> const& position,
			    Color<GLfloat> const& color,
			    Color<GLfloat> const& specularColor)
  : gle::Light(gle::Light::POINT), _position(position)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
  _specularColor[0] = specularColor.r;
  _specularColor[1] = specularColor.g;
  _specularColor[2] = specularColor.b;
}

gle::PointLight::PointLight(Vector3<GLfloat> const& position,
                            Color<GLfloat> const& color)
  : gle::Light(gle::Light::POINT), _position(position)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
  _specularColor[0] = color.r;
  _specularColor[1] = color.g;
  _specularColor[2] = color.b;
}

gle::PointLight::~PointLight()
{

}

void gle::PointLight::setPosition(Vector3<GLfloat> const& position)
{
  _position = position;
}

void gle::PointLight::setColor(Color<GLfloat> const& color)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
}

void gle::PointLight::setSpecularColor(Color<GLfloat> const& specularColor)
{
  _specularColor[0] = specularColor.r;
  _specularColor[1] = specularColor.g;
  _specularColor[2] = specularColor.b;
}

gle::Vector3<GLfloat>& gle::PointLight::getPosition()
{
  return (_position);
}

GLfloat* gle::PointLight::getColor()
{
  return (_color);
}

GLfloat* gle::PointLight::getSpecularColor()
{
  return (_specularColor);
}
