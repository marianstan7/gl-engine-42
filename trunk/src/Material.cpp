//
// Material.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:50:35 2012 gael jochaud-du-plessix
// Last update Thu May 31 00:00:21 2012 gael jochaud-du-plessix
//

#include <Material.hpp>
#include <Scene.hpp>
#include <ShaderSource.hpp>
#include <sstream>

gle::Material::Material(std::string const & name) :
  _name(name),
  _ambientColor(0.0, 0.0, 0.0), _diffuseColor(1.0, 1.0, 1.0),
  _specularColor(1.0, 1.0, 1.0),
  _diffuseLightEnabled(true), _specularLightEnabled(true),
  _shininess(50), _diffuseIntensity(1), _specularIntensity(1),
  _colorMapEnabled(false), _colorMap(NULL), _uniforms(NULL), _needUniformsUpdate(true)
{
  _uniforms = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::UniformArray,
                                       gle::Buffer<GLfloat>::StaticDraw);
  _uniforms->resize(15);
}

gle::Material::~Material()
{
  if (_uniforms)
    delete _uniforms;
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
  _needUniformsUpdate = true;
}

gle::Color<GLfloat> const & gle::Material::getAmbientColor() const
{
  return (_ambientColor);
}

void gle::Material::setDiffuseColor(gle::Color<GLfloat> const & color)
{
  _diffuseColor = color;
  _needUniformsUpdate = true;
}

gle::Color<GLfloat> const & gle::Material::getDiffuseColor() const
{
  return (_diffuseColor);
}

void gle::Material::setSpecularColor(gle::Color<GLfloat> const & color)
{
  _specularColor = color;
  _needUniformsUpdate = true;
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
  _needUniformsUpdate = true;
}

void gle::Material::setSpecularLightEnabled(bool enabled)
{
  _specularLightEnabled = enabled;
  if (_specularIntensity == 0)
    _specularIntensity = 1;
  _needUniformsUpdate = true;
}

void gle::Material::setShininess(GLfloat shininess)
{
  _shininess = shininess;
  if (_specularIntensity == 0)
    _specularIntensity = 1.0;
  _needUniformsUpdate = true;
}

GLfloat gle::Material::getShininess() const
{
  return (_shininess);
}

void gle::Material::setDiffuseIntensity(GLfloat diffuseIntensity)
{
  _diffuseIntensity = diffuseIntensity;
  _needUniformsUpdate = true;
}

void gle::Material::setSpecularIntensity(GLfloat specularIntensity)
{
  _specularIntensity = specularIntensity;
  _needUniformsUpdate = true;
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
  _needUniformsUpdate = true;
}

gle::Texture* gle::Material::getColorMap() const
{
  return (_colorMap);
}

void gle::Material::setColorMap(gle::Texture* colorMap)
{
  _colorMap = colorMap;
  _colorMapEnabled = colorMap != NULL ? true : false;
  _needUniformsUpdate = true;
}

gle::Buffer<GLfloat>* gle::Material::getUniforms()
{
  if (_needUniformsUpdate)
    {
      GLfloat color[15];
      for (int i = 0; i < 15; i++)
	color[i] = 0;
      color[0] = _ambientColor.r;
      color[1] = _ambientColor.g;
      color[2] = _ambientColor.b;
      color[3] = 1.0;
      color[4] = _diffuseColor.r;
      color[5] = _diffuseColor.g;
      color[6] = _diffuseColor.b;
      color[7] = 1.0;
      color[8] = _specularColor.r;
      color[9] = _specularColor.g;
      color[10] = _specularColor.b;
      color[11] = 1.0;
      color[12] = _shininess;
      color[13] = getSpecularIntensity();
      color[14] = getDiffuseIntensity();
      _uniforms->setData((const GLfloat*)color);
      _needUniformsUpdate = false;
    }
  return (_uniforms);
}
