//
// Material.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 22:50:35 2012 gael jochaud-du-plessix
// Last update Tue Feb 28 16:22:13 2012 loick michard
//

#include <Material.hpp>
#include <Scene.hpp>
#include <ShaderSource.hpp>
#include <sstream>

gle::Material::Material() :
  _colorEnabled(false), _isLight(false),
  _shininess(0), _specularIntensity(0)
{
}

gle::Material::~Material()
{
}

void gle::Material::setColorEnabled(bool colorEnabled)
{
  _colorEnabled = colorEnabled;
}

bool gle::Material::isColorEnabled() const
{
  return (_colorEnabled);
}

void gle::Material::isLight(bool isLight)
{
  _isLight = isLight;
}

bool gle::Material::isLight() const
{
  return (_isLight);
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

void gle::Material::setSpecularIntensity(GLfloat specularIntensity)
{
  _specularIntensity = specularIntensity;
}

GLfloat gle::Material::getSpecularIntensity() const
{
  return (_specularIntensity);
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
  if (!_isLight && scene->isLightEnabled() &&
      (scene->getDirectionalLightsSize() || scene->getPointLightsSize()))
    program->getUniformLocation(gle::Program::NMatrix);
  if (!_isLight && scene->isLightEnabled())
    {
      program->getUniformLocation(gle::Program::ambientColor);
      program->getUniformLocation(gle::Program::shininess);
      program->getUniformLocation(gle::Program::specularIntensity);
      if (scene->getDirectionalLightsSize())
	{
	  program->getUniformLocation(gle::Program::directionalLightDirection);
	  program->getUniformLocation(gle::Program::directionalLightColor);
	}
      if (scene->getPointLightsSize())
	{
	  program->getUniformLocation(gle::Program::pointLightPosition);
	  program->getUniformLocation(gle::Program::pointLightColor);	  
	  program->getUniformLocation(gle::Program::pointLightSpecularColor);
	}
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
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::Head;
  if (!_isLight && scene->isLightEnabled())
    shaderSource += _replace("%nb_point_lights", 
			     scene->getPointLightsSize(),
			     _replace("%nb_directional_lights",
				      scene->getDirectionalLightsSize(),
				      gle::ShaderSource::Vertex::Light::Head));
  
  // Input locations
  shaderSource += gle::ShaderSource::Vertex::Default::InputLocations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::InputLocations;
  // Output locations
  shaderSource += gle::ShaderSource::Vertex::Default::OutputLocations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::OutputLocations;

  // Uniform declarations
  shaderSource += gle::ShaderSource::Vertex::Default::UniformDeclarations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::UniformDeclarations;
  if (!_isLight && scene->isLightEnabled())
    shaderSource += gle::ShaderSource::Vertex::Light::UniformDeclarations;
  // Input declarations
  shaderSource += gle::ShaderSource::Vertex::Default::InputDeclarations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::InputDeclarations;
  
  // Output declarations
  shaderSource += gle::ShaderSource::Vertex::Default::OutputDeclarations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::OutputDeclarations;

  // Shader body
  shaderSource += gle::ShaderSource::Vertex::BodyBegin;
  
  shaderSource += gle::ShaderSource::Vertex::Default::Body;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Vertex::Color::Body;
  if (!_isLight && scene->isLightEnabled())
    shaderSource += gle::ShaderSource::Vertex::Light::Body;

  shaderSource += gle::ShaderSource::Vertex::BodyEnd;
  std::cout << shaderSource << std::endl;
  gle::Shader *shader = new gle::Shader(gle::Shader::Vertex, shaderSource);

  return (shader);
}

gle::Shader* gle::Material::_createFragmentShader(Scene* scene)
{
  (void)scene;
  std::string shaderSource;

  // Headers
  shaderSource += gle::ShaderSource::Fragment::Default::Head;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::Head;

  // Input locations
  shaderSource += gle::ShaderSource::Fragment::Default::InputLocations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::InputLocations;
  // Output locations
  shaderSource += gle::ShaderSource::Fragment::Default::OutputLocations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::OutputLocations;

  // Input declarations
  shaderSource += gle::ShaderSource::Fragment::Default::InputDeclarations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::InputDeclarations;
  
  // Output declarations
  shaderSource += gle::ShaderSource::Fragment::Default::OutputDeclarations;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::OutputDeclarations;

  // Shader body
  shaderSource += gle::ShaderSource::Fragment::BodyBegin;
  
  shaderSource += gle::ShaderSource::Fragment::Default::Body;
  if (_isLight || _colorEnabled)
    shaderSource += gle::ShaderSource::Fragment::Color::Body;

  shaderSource += gle::ShaderSource::Fragment::BodyEnd;
  std::cout << shaderSource << std::endl;
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
