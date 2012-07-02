//
// SpotLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Mon Jul  2 16:36:14 2012 gael jochaud-du-plessix
//

#include <SpotLight.hpp>

#include <PerspectiveCamera.hpp>

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
			  Color<GLfloat> const& specularColor,
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
  _cosCutOff = cosCutOff;
  _target = gle::Vector3f(0.0, -1.0, 0.0);
  _hasTarget = true;
}

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
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
  _cosCutOff = cosCutOff;
  _target = gle::Vector3f(0.0, -1.0, 0.0);
  _hasTarget = true;
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

void gle::SpotLight::setCosCutOff(GLfloat cosCutOff)
{
  _cosCutOff = cosCutOff;
  if (_shadowMapCamera)    
    dynamic_cast<gle::PerspectiveCamera*>(_shadowMapCamera)
      ->setFovy(_cosCutOff / M_PI * 180.0);
  else
    getShadowMapCamera();
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

GLfloat gle::SpotLight::getCosCutOff()
{
  return (_cosCutOff);
}

gle::Camera* gle::SpotLight::getShadowMapCamera()
{
  if (!_shadowMapCamera)
    {
      _shadowMapCamera = 
	new gle::PerspectiveCamera(getPosition(),
				   getTarget(),
				   _cosCutOff / M_PI * 180.0,
				   ((GLfloat)_shadowMapSize.width
				    /_shadowMapSize.height),
				   50, 1000);
    }
  return (_shadowMapCamera);
}

void gle::SpotLight::update()
{
  if (_shadowMapCamera)
    {
      _shadowMapCamera->setPosition(_position);
      _shadowMapCamera->setTarget(_target);
    }
}
