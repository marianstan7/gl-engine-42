//
// SpotLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Wed Jun  6 17:31:34 2012 loick michard
//

#include <SpotLight.hpp>

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
			  Color<GLfloat> const& specularColor,
			  gle::Vector3f const& direction,
			  GLfloat cosCutOff)
  : gle::Light(gle::Light::SPOT)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
  _specularColor[0] = specularColor.r;
  _specularColor[1] = specularColor.g;
  _specularColor[2] = specularColor.b;
  _position = position;
  _attenuation[0] = _attenuation[1] = _attenuation[2] = 0;
  _direction[0] = direction.x;
  _direction[1] = direction.y;
  _direction[2] = direction.z;
  _cosCutOff = cosCutOff;
}

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
			  gle::Vector3f const& direction,
			  GLfloat cosCutOff)
  : gle::Light(gle::Light::SPOT)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
  _specularColor[0] = color.r;
  _specularColor[1] = color.g;
  _specularColor[2] = color.b;
  _position = position;
  _attenuation[0] = _attenuation[1] = _attenuation[2] = 0;
  _direction[0] = direction.x;
  _direction[1] = direction.y;
  _direction[2] = direction.z;
  _cosCutOff = cosCutOff;
}

gle::SpotLight::~SpotLight()
{

}

void gle::SpotLight::setColor(Color<GLfloat> const& color)
{
  _color[0] = color.r;
  _color[1] = color.g;
  _color[2] = color.b;
}

void gle::SpotLight::setSpecularColor(Color<GLfloat> const& specularColor)
{
  _specularColor[0] = specularColor.r;
  _specularColor[1] = specularColor.g;
  _specularColor[2] = specularColor.b;
}

void gle::SpotLight::setAttenuation(GLfloat constant, GLfloat linear, GLfloat quadratic)
{
  _attenuation[0] = constant;
  _attenuation[1] = linear;
  _attenuation[2] = quadratic;
}

GLfloat* gle::SpotLight::getColor()
{
  return (_color);
}

GLfloat* gle::SpotLight::getSpecularColor()
{
  return (_specularColor);
}

GLfloat* gle::SpotLight::getAttenuation()
{
  return (_attenuation);
}

void gle::SpotLight::update()
{

}
