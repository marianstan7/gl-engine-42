//
// Material.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:50:35 2012 gael jochaud-du-plessix
// Last update Thu Jun 21 00:55:07 2012 loick michard
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
  _colorMapEnabled(false), _colorMap(NULL),
  _normalMapEnabled(false), _normalMap(NULL),
  _envMapEnabled(false), _envMap(NULL),
  _uniforms(NULL), _uniformsBuffer(NULL),
  _needUniformsUpdate(true), _needUniformsBufferUpdate(true)
{
  _uniforms = new GLfloat[UniformSize];
  _uniformsBuffer = new gle::Buffer<GLfloat>(gle::Buffer<GLfloat>::UniformArray,
					     gle::Buffer<GLfloat>::StaticDraw);
  _uniformsBuffer->resize(UniformSize);
}

gle::Material::~Material()
{
  if (_uniforms)
    delete[] _uniforms;
  if (_uniformsBuffer)
    delete _uniformsBuffer;
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

void gle::Material::setReflectionIntensity(GLfloat reflectionIntensity)
{
  _reflectionIntensity = reflectionIntensity;
  _needUniformsUpdate = true;
}

GLfloat gle::Material::getReflectionIntensity() const
{
  return (_reflectionIntensity);
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

bool gle::Material::isNormalMapEnabled() const
{
  return (_normalMapEnabled);
}

void gle::Material::setNormalMapEnabled(bool enabled)
{
  _normalMapEnabled = enabled;
  _needUniformsUpdate = true;
}

gle::Texture* gle::Material::getNormalMap() const
{
  return (_normalMap);
}

void gle::Material::setNormalMap(gle::Texture* normalMap)
{
  _normalMap = normalMap;
  _normalMapEnabled = normalMap != NULL ? true : false;
  _needUniformsUpdate = true;
}

bool gle::Material::isEnvMapEnabled() const
{
  return (_envMapEnabled);
}

void gle::Material::setEnvEnabled(bool enabled)
{
  _envMapEnabled = enabled;
}

void gle::Material::setEnvMap(gle::EnvironmentMap* envMap)
{
  _envMap = envMap;
  _envMapEnabled = true;
}

gle::EnvironmentMap* gle::Material::getEnvMap() const
{
  return (_envMap);
}

gle::Buffer<GLfloat>* gle::Material::getUniformsBuffer() const
{
  if (_needUniformsUpdate || _needUniformsBufferUpdate)
    {
      _uniformsBuffer->setData(getUniforms());
      _needUniformsBufferUpdate = false;
    }
  return (_uniformsBuffer);
}

const GLfloat* gle::Material::getUniforms() const
{
  if (_needUniformsUpdate)
    {
      for (int i = 0; i < UniformSize; i++)
	_uniforms[i] = 0;
      _uniforms[0] = _ambientColor.r;
      _uniforms[1] = _ambientColor.g;
      _uniforms[2] = _ambientColor.b;
      _uniforms[3] = 1.0;
      _uniforms[4] = _diffuseColor.r;
      _uniforms[5] = _diffuseColor.g;
      _uniforms[6] = _diffuseColor.b;
      _uniforms[7] = 1.0;
      _uniforms[8] = _specularColor.r;
      _uniforms[9] = _specularColor.g;
      _uniforms[10] = _specularColor.b;
      _uniforms[11] = 1.0;
      _uniforms[12] = _shininess;
      _uniforms[13] = getSpecularIntensity();
      _uniforms[14] = getDiffuseIntensity();
      _uniforms[15] = _reflectionIntensity;
      _uniforms[16] = (_envMapEnabled && _envMap) ? _envMap->getType() : 0;
      _uniforms[17] = (_colorMapEnabled && _colorMap) ? 1.0 : 0.0;
      _uniforms[18] = (_normalMapEnabled && _normalMap) ? 1.0 : 0.0;
      _needUniformsUpdate = false;
    }
  return (_uniforms);
}

bool gle::Material::canBeRenderedWith(const gle::Material* other) const
{
  if ((!_colorMapEnabled || !other->isColorMapEnabled()
       || _colorMap == other->getColorMap()) &&
      (!_normalMapEnabled || !other->isNormalMapEnabled()
       || _normalMap == other->getNormalMap()) &&
      (!_envMapEnabled || !other->isEnvMapEnabled()
       || _envMap == other->getEnvMap()))
    return (true);
  return (false);
}
