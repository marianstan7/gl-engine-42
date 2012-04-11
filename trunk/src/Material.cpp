//
// Material.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:50:35 2012 gael jochaud-du-plessix
// Last update Wed Apr 11 23:20:23 2012 loick michard
//

#include <Material.hpp>
#include <Scene.hpp>
#include <ShaderSource.hpp>
#include <sstream>

gle::Material::Material(std::string const & name) :
  _name(name),
  _ambientColor(0.0, 0.0, 0.0), _diffuseColor(1.0, 1.0, 1.0),
  _specularColor(1.0, 1.0, 1.0),
  _diffuseLightEnabled(false), _specularLightEnabled(false),
  _shininess(50), _diffuseIntensity(1), _specularIntensity(1),
  _colorMapEnabled(false), _colorMap(NULL)
{
}

gle::Material::~Material()
{
}

void gle::Material::setName(std::string const & name)
{
  _name = name;
}

std::string const & gle::Material::getName() const
{
  return (_name);
}

void gle::Material::setAmbientColor(gle::Color<GLfloat> const & color)
{
  _ambientColor = color;
}

gle::Color<GLfloat> const & gle::Material::getAmbientColor() const
{
  //return(_colorMapEnabled ? gle::Color<GLfloat>(0, 0, 0) : _ambientColor);
  return (_ambientColor);
}

void gle::Material::setDiffuseColor(gle::Color<GLfloat> const & color)
{
  _diffuseColor = color;
}

gle::Color<GLfloat> const & gle::Material::getDiffuseColor() const
{
  return (_diffuseColor);
}

void gle::Material::setSpecularColor(gle::Color<GLfloat> const & color)
{
  _specularColor = color;
}

gle::Color<GLfloat> const & gle::Material::getSpecularColor() const
{
  return (_specularColor);
}

bool gle::Material::isDiffuseLightEnabled() const
{
  return (_diffuseLightEnabled);
}

bool gle::Material::isSpecularLightEnabled() const
{
  return (_specularLightEnabled);
}

void gle::Material::setDiffuseLightEnabled(bool enabled)
{
  _diffuseLightEnabled = enabled;
  if (_diffuseIntensity == 0)
    _diffuseIntensity = 1;
}

void gle::Material::setSpecularLightEnabled(bool enabled)
{
  _specularLightEnabled = enabled;
  if (_specularIntensity == 0)
    _specularIntensity = 1;
}

void gle::Material::setShininess(GLfloat shininess)
{
  _shininess = shininess;
  if (_specularIntensity == 0)
    _specularIntensity = 1.0;
}

GLfloat gle::Material::getShininess() const
{
  return (_shininess);
}

void gle::Material::setDiffuseIntensity(GLfloat diffuseIntensity)
{
  _diffuseIntensity = diffuseIntensity;
}

void gle::Material::setSpecularIntensity(GLfloat specularIntensity)
{
  _specularIntensity = specularIntensity;
}

GLfloat gle::Material::getDiffuseIntensity() const
{
  return (_diffuseLightEnabled ? _diffuseIntensity : 0);
}

GLfloat gle::Material::getSpecularIntensity() const
{
  return (_specularLightEnabled ? _specularIntensity : 0);
}

bool gle::Material::isColorMapEnabled() const
{
  return (_colorMapEnabled);
}

void gle::Material::setColorMapEnabled(bool enabled)
{
  _colorMapEnabled = enabled;
}

gle::Texture* gle::Material::getColorMap() const
{
  return (_colorMap);
}

void gle::Material::setColorMap(gle::Texture* colorMap)
{
  _colorMap = colorMap;
  _colorMapEnabled = true;
}
