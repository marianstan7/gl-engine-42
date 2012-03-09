//
// Material.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:50:35 2012 gael jochaud-du-plessix
// Last update Fri Mar  9 20:35:03 2012 gael jochaud-du-plessix
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

gle::Program* gle::Material::createProgram(Scene* scene)
{  
  gle::Shader* vertexShader = _createVertexShader(scene);
  gle::Shader* fragmentShader = _createFragmentShader(scene);
  gle::Program* program = new gle::Program();

  program->attach(*vertexShader);
  program->attach(*fragmentShader);

  try {
    program->link();
  }
  catch (std::exception *e)
    {
      delete vertexShader;
      delete fragmentShader;
      throw e;
    }

  // Get uniform locations
  program->getUniformLocation(gle::Program::MVMatrix);
  program->getUniformLocation(gle::Program::PMatrix);
  program->getUniformLocation(gle::Program::AmbientColor);
  program->getUniformLocation(gle::Program::DiffuseColor);
  program->getUniformLocation(gle::Program::SpecularColor);
  if (isColorMapEnabled())
    program->getUniformLocation(gle::Program::ColorMap);
  program->getUniformLocation(gle::Program::NMatrix);

  program->getUniformLocation(gle::Program::Shininess);
  program->getUniformLocation(gle::Program::SpecularIntensity);
  program->getUniformLocation(gle::Program::DiffuseIntensity);

  if (scene->getDirectionalLightsSize())
    {
      program->getUniformLocation(gle::Program::DirectionalLightDirection);
      program->getUniformLocation(gle::Program::DirectionalLightColor);
    }
  if (scene->getPointLightsSize())
    {
      program->getUniformLocation(gle::Program::PointLightPosition);
      program->getUniformLocation(gle::Program::PointLightColor);	  
      program->getUniformLocation(gle::Program::PointLightSpecularColor);
    }

  delete vertexShader;
  delete fragmentShader;

  return (program);
}

gle::Shader* gle::Material::_createVertexShader(Scene* scene)
{
  (void)scene;
  std::string shaderSource;
  
  // Headers
  shaderSource += gle::ShaderSource::Vertex::Default::Head;

  std::string head = gle::ShaderSource::Vertex::Light::Head;
  head = _replace("%nb_directional_lights",
		  scene->getDirectionalLightsSize(), head);
  head += _replace("%nb_point_lights", scene->getPointLightsSize(), head);
  shaderSource += head;
  
  // Input locations
  shaderSource += gle::ShaderSource::Vertex::Default::InputLocations;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Vertex::ColorMap::InputLocations;

  // Output locations
  shaderSource += gle::ShaderSource::Vertex::Default::OutputLocations;

  // Uniform declarations
  shaderSource += gle::ShaderSource::Vertex::Default::UniformDeclarations;
  shaderSource += gle::ShaderSource::Vertex::Light::UniformDeclarations;

  // Input declarations
  shaderSource += gle::ShaderSource::Vertex::Default::InputDeclarations;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Vertex::ColorMap::InputDeclarations;
  
  // Output declarations
  shaderSource += gle::ShaderSource::Vertex::Default::OutputDeclarations;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Vertex::ColorMap::OutputDeclarations;

  // Shader body
  shaderSource += gle::ShaderSource::Vertex::BodyBegin;  
  shaderSource += gle::ShaderSource::Vertex::Default::Body;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Vertex::ColorMap::Body;
  shaderSource += gle::ShaderSource::Vertex::Light::Body;

  shaderSource += gle::ShaderSource::Vertex::BodyEnd;

  gle::Shader *shader = new gle::Shader(gle::Shader::Vertex, shaderSource);

  return (shader);
}

gle::Shader* gle::Material::_createFragmentShader(Scene* scene)
{
  (void)scene;
  std::string shaderSource;

  // Headers
  shaderSource += gle::ShaderSource::Fragment::Default::Head;

  // Uniform declarations
  shaderSource += gle::ShaderSource::Fragment::Default::UniformDeclarations;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Fragment::ColorMap::UniformDeclarations;

  // Input locations
  shaderSource += gle::ShaderSource::Fragment::Default::InputLocations;

  // Output locations
  shaderSource += gle::ShaderSource::Fragment::Default::OutputLocations;

  // Input declarations
  shaderSource += gle::ShaderSource::Fragment::Default::InputDeclarations;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Fragment::ColorMap::InputDeclarations;
  shaderSource += gle::ShaderSource::Fragment::Light::InputDeclarations;
  
  // Output declarations
  shaderSource += gle::ShaderSource::Fragment::Default::OutputDeclarations;

  // Shader body
  shaderSource += gle::ShaderSource::Fragment::BodyBegin;
  shaderSource += gle::ShaderSource::Fragment::Default::Body;
  if (isColorMapEnabled())
    shaderSource += gle::ShaderSource::Fragment::ColorMap::Body;
  shaderSource += gle::ShaderSource::Fragment::Light::Body;

  shaderSource += gle::ShaderSource::Fragment::BodyEnd;
  gle::Shader *shader = new gle::Shader(gle::Shader::Fragment, shaderSource);

  return (shader);
}

std::string gle::Material::_replace(std::string const& search,
				    int to,
				    std::string const& str)
{
  std::stringstream ss;

  ss << to;
  std::string ret = str;
  return (ret.replace(str.find(search), search.size(), ss.str()));
}
