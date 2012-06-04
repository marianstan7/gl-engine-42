//
// PointLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Mon Jun  4 14:59:48 2012 loick michard
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
  _position = position;
  _attenuation[0] = _attenuation[1] = _attenuation[2] = 0;
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
  _position = position;
  _attenuation[0] = _attenuation[0];
  _attenuation[1] = _attenuation[1];
  _attenuation[2] = _attenuation[2];
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

void gle::PointLight::setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic)
{
  _attenuation[0] = constant;
  _attenuation[1] = linear;
  _attenuation[2] = quadratic;
}

GLfloat* gle::PointLight::getColor()
{
  return (_color);
}

GLfloat* gle::PointLight::getSpecularColor()
{
  return (_specularColor);
}

GLfloat* gle::PointLight::getAttenuation()
{
  return (_attenuation);
}

void gle::PointLight::update()
{

}
