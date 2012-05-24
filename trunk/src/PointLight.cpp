//
// PointLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Thu May 24 17:31:50 2012 loick michard
//

#include <PointLight.hpp>

gle::PointLight::PointLight(Vector3<GLfloat> const& position,
			    Color<GLfloat> const& color,
			    Color<GLfloat> const& specularColor)
  : gle::Light(gle::Light::POINT)
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
  : gle::Light(gle::Light::POINT)
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

GLfloat* gle::PointLight::getColor()
{
  return (_color);
}

GLfloat* gle::PointLight::getSpecularColor()
{
  return (_specularColor);
}

void gle::PointLight::update()
{

}
