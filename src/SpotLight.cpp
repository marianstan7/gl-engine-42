//
// SpotLight.cpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Feb 23 17:55:31 2012 loick michard
// Last update Fri Jul  6 01:41:21 2012 loick michard
//

#include <SpotLight.hpp>

#include <PerspectiveCamera.hpp>

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
			  Color<GLfloat> const& specularColor,
			  GLfloat cutOff)
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
  _cosCutOff = cos(cutOff * M_PI / 180);
  _innerCosCutOff = _cosCutOff;
  _target = gle::Vector3f(0.0, -1.0, 0.0);
  _hasTarget = true;
}

gle::SpotLight::SpotLight(Vector3<GLfloat> const& position,
			  Color<GLfloat> const& color,
			  GLfloat cutOff)
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
  _cosCutOff = cos(cutOff * M_PI / 180);
  _innerCosCutOff = _cosCutOff;
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

void gle::SpotLight::setCutOff(GLfloat cutOff)
{
  _cosCutOff = cos(cutOff * M_PI / 180);
  if (_shadowMapCamera)    
    dynamic_cast<gle::PerspectiveCamera*>(_shadowMapCamera)
      ->setFovy(acos(_cosCutOff) / M_PI * 360.0);
  else
    getShadowMapCamera();
}

void gle::SpotLight::setInnerCutOff(GLfloat innerCutOff)
{
  _innerCosCutOff = cos(innerCutOff * M_PI / 180);
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

GLfloat gle::SpotLight::getCutOff()
{
  return (acos(_cosCutOff) / M_PI * 180);
}

GLfloat gle::SpotLight::getInnerCutOff()
{
  return (acos(_innerCosCutOff) / M_PI * 180);
}

GLfloat gle::SpotLight::getCosCutOff()
{
  return (_cosCutOff);
}

GLfloat gle::SpotLight::getInnerCosCutOff()
{
  return (_innerCosCutOff);
}

gle::Camera* gle::SpotLight::getShadowMapCamera()
{
  if (!_shadowMapCamera)
    {
      _shadowMapCamera = 
	new gle::PerspectiveCamera(getPosition(),
				   getTarget(),
				   acos(_cosCutOff) / M_PI * 360.0,
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
