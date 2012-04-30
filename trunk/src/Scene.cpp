//
// Scene.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 19:12:49 2012 gael jochaud-du-plessix
// Last update Mon Apr 30 15:39:46 2012 gael jochaud-du-plessix
//

#include <Scene.hpp>
#include <algorithm>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <ShaderSource.hpp>
#include <sstream>

gle::Scene::Scene() :
  _backgroundColor(0.0, 0.0, 0.0, 0.0),
  _cameras(), _meshes(), _materials(),
  _lights(), _directionalLightsDirection(),
  _directionalLightsColor(), _directionalLightsSize(0),
  _pointLightsPosition(),
  _pointLightsColor(), _pointLightsSize(0),
  _currentCamera(NULL), _program(NULL), _needProgramCompilation(true)
{

}

gle::Scene::~Scene()
{

}

void gle::Scene::setBackgroundColor(gle::Color<GLfloat> const &color)
{
  _backgroundColor = color;
}

gle::Color<GLfloat> const & gle::Scene::getBackgroundColor() const
{
  return (_backgroundColor);
}

gle::Scene & gle::Scene::add(Camera* camera)
{  
  if (find(_cameras.begin(), _cameras.end(), camera) == _cameras.end())
    {
      if (!_currentCamera)
	_currentCamera = camera;
      _cameras.push_back(camera);
    }
  return (*this);
}

gle::Scene & gle::Scene::add(Mesh* mesh)
{
  if (!mesh)
    return (*this);
  add(mesh->getMaterial());
  if (find(_meshes.begin(), _meshes.end(), mesh) == _meshes.end())
    _meshes.push_back(mesh);
  std::vector<gle::Mesh*> & childs = mesh->getChildren();
  add(childs);
  return (*this);
}

gle::Scene & gle::Scene::add(std::vector<gle::Mesh*> const & meshes)
{
  for (std::vector<gle::Mesh*>::const_iterator it = meshes.begin();
       it != meshes.end(); ++it)
    add(*it);
  return (*this);
}

gle::Scene & gle::Scene::add(Material* material)
{
  if (!material)
    return (*this);
  if (find(_materials.begin(), _materials.end(), material) == _materials.end())
    _materials.push_back(material);
  return (*this);
}

gle::Scene & gle::Scene::add(Light* light)
{
  if (!light)
    return (*this);
  if (find(_lights.begin(), _lights.end(), light) == _lights.end())
    {
      _lights.push_back(light);
      this->updateLights();
      _needProgramCompilation = true;
    }
  return (*this);
}

gle::Scene & gle::Scene::remove(Camera* camera)
{
  std::vector<Camera*>::iterator it;

  if ((it = find(_cameras.begin(), _cameras.end(), camera)) != _cameras.end())
    _cameras.erase(it);
  return (*this);
}

gle::Scene & gle::Scene::remove(Mesh* mesh)
{
  std::vector<Mesh*>::iterator it;

  if ((it = find(_meshes.begin(), _meshes.end(), mesh)) != _meshes.end())
    _meshes.erase(it);
  return (*this);
}

gle::Scene & gle::Scene::remove(Material* material)
{
  std::vector<Material*>::iterator it;

  if ((it = find(_materials.begin(), _materials.end(), material)) != _materials.end())
    _materials.erase(it);
  return (*this);
}

gle::Scene & gle::Scene::remove(Light* light)
{
  std::vector<Light*>::iterator it;

  if ((it = find(_lights.begin(), _lights.end(), light)) != _lights.end())
    {
      _lights.erase(it);
      this->updateLights();
      _needProgramCompilation = true;
    }
  return (*this);
}

std::vector<gle::Mesh*> & gle::Scene::getMeshes()
{
  return (_meshes);
}

std::vector<gle::Camera*> & gle::Scene::getCameras()
{
  return (_cameras);
}

std::vector<gle::Material*> & gle::Scene::getMaterials()
{
  return (_materials);
}

std::vector<gle::Light*> & gle::Scene::getLights()
{
  return (_lights);
}

gle::Camera* gle::Scene::getCurrentCamera()
{
  return (_currentCamera);
}

void gle::Scene::updateLights()
{
  _directionalLightsDirection.resize(0);
  _directionalLightsColor.resize(0);

  _pointLightsPosition.resize(0);
  _pointLightsColor.resize(0);
  _pointLightsSpecularColor.resize(0);
  unsigned int dSize = 0;
  unsigned int pSize = 0;
  for (std::vector<Light*>::iterator it = _lights.begin();
       it != _lights.end(); ++it)
    {
      if ((*it)->getType() == gle::Light::DIRECTIONAL)
	{
	  ++dSize;
	  gle::Vector3<GLfloat> direction = ((gle::DirectionalLight*)(*it))->getDirection();
	  GLfloat* color = ((gle::DirectionalLight*)(*it))->getColor();
	  _directionalLightsDirection.push_back(direction.x);
	  _directionalLightsDirection.push_back(direction.y);
	  _directionalLightsDirection.push_back(direction.z);
	  _directionalLightsColor.push_back(color[0]);
	  _directionalLightsColor.push_back(color[1]);
	  _directionalLightsColor.push_back(color[2]);
	}
      else if ((*it)->getType() == gle::Light::POINT)
	{
	  ++pSize;
	  gle::Vector3<GLfloat> position = ((gle::PointLight*)(*it))->getPosition();
	  position *= _currentCamera->getModelViewMatrix();
          GLfloat* color = ((gle::PointLight*)(*it))->getColor();
          GLfloat* specularColor = ((gle::PointLight*)(*it))->getSpecularColor();
          _pointLightsPosition.push_back(position.x);
          _pointLightsPosition.push_back(position.y);
          _pointLightsPosition.push_back(position.z);
          _pointLightsColor.push_back(color[0]);
          _pointLightsColor.push_back(color[1]);
          _pointLightsColor.push_back(color[2]);
          _pointLightsSpecularColor.push_back(specularColor[0]);
          _pointLightsSpecularColor.push_back(specularColor[1]);
          _pointLightsSpecularColor.push_back(specularColor[2]);
	}
    }
  _directionalLightsSize = dSize;
  _pointLightsSize = pSize;
}

GLfloat* gle::Scene::getDirectionalLightsDirection() const
{
  return ((GLfloat*)&_directionalLightsDirection[0]);
}

GLfloat* gle::Scene::getDirectionalLightsColor() const
{
  return ((GLfloat*)&_directionalLightsColor[0]);
}

GLsizeiptr gle::Scene::getDirectionalLightsSize() const
{
  return (_directionalLightsSize);
}

GLfloat* gle::Scene::getPointLightsPosition() const
{
  return ((GLfloat*)&_pointLightsPosition[0]);
}

GLfloat* gle::Scene::getPointLightsColor() const
{
  return ((GLfloat*)&_pointLightsColor[0]);
}

GLfloat* gle::Scene::getPointLightsSpecularColor() const
{
  return ((GLfloat*)&_pointLightsSpecularColor[0]);
}

GLsizeiptr gle::Scene::getPointLightsSize() const
{
  return (_pointLightsSize);
}

bool gle::Scene::hasLights() const
{
  return (getDirectionalLightsSize() || getPointLightsSize());
}

void		gle::Scene::buildProgram()
{
  _needProgramCompilation = false;
  if (_program)
    delete(_program);
  _program = new gle::Program();

  gle::Shader* vertexShader = _createVertexShader();
  gle::Shader* fragmentShader = _createFragmentShader();

  _program->attach(*vertexShader);
  _program->attach(*fragmentShader);

  try {
    _program->link();
  }
  catch (std::exception *e)
    {
      delete vertexShader;
      delete fragmentShader;
      throw e;
    }

  _program->getUniformLocation(gle::Program::MVMatrix);
  _program->getUniformLocation(gle::Program::PMatrix);
  /*  _program->getUniformLocation(gle::Program::AmbientColor);
  _program->getUniformLocation(gle::Program::DiffuseColor);
  _program->getUniformLocation(gle::Program::SpecularColor);*/
  _program->getUniformLocation(gle::Program::HasColorMap);
  _program->getUniformLocation(gle::Program::ColorMap);
  _program->getUniformLocation(gle::Program::NMatrix);
  /*_program->getUniformLocation(gle::Program::Shininess);
  _program->getUniformLocation(gle::Program::SpecularIntensity);
  _program->getUniformLocation(gle::Program::DiffuseIntensity);*/
  if (getDirectionalLightsSize())
    {
      _program->getUniformLocation(gle::Program::DirectionalLightDirection);
      _program->getUniformLocation(gle::Program::DirectionalLightColor);
    }
  if (getPointLightsSize())
    {
      _program->getUniformLocation(gle::Program::PointLightPosition);
      _program->getUniformLocation(gle::Program::PointLightColor);
      _program->getUniformLocation(gle::Program::PointLightSpecularColor);
    }
  _program->retreiveUniformBlockIndex(gle::Program::MaterialBlock, "materialBlock");
  delete vertexShader;
  delete fragmentShader;
}

gle::Shader* gle::Scene::_createVertexShader()
{
  std::string shaderSource;

  shaderSource += gle::ShaderSource::VertexShader;
  shaderSource = _replace("%nb_directional_lights", getDirectionalLightsSize(), shaderSource);
  shaderSource =_replace("%nb_point_lights", getPointLightsSize(), shaderSource);
  gle::Shader *shader = new gle::Shader(gle::Shader::Vertex, shaderSource);

  return (shader);
}

gle::Shader* gle::Scene::_createFragmentShader()
{
  std::string shaderSource;

  shaderSource += gle::ShaderSource::FragmentShader;
  gle::Shader *shader = new gle::Shader(gle::Shader::Fragment, shaderSource);

  return (shader);
}

std::string gle::Scene::_replace(std::string const& search,
				int to,
				std::string const& str)
{
  std::stringstream ss;

  ss << to;
  std::string ret = str;
  return (ret.replace(str.find(search), search.size(), ss.str()));
}

gle::Program*	gle::Scene::getProgram()
{
  if (_needProgramCompilation)
    this->buildProgram();
  return (_program);
}
