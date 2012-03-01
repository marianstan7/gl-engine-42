//
// Scene.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 19:12:49 2012 gael jochaud-du-plessix
// Last update Wed Feb 29 16:15:59 2012 loick michard
//

#include <Scene.hpp>
#include <algorithm>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

gle::Scene::Scene() :
  _cameras(), _meshes(), _materials(),
  _lights(), _directionalLightsDirection(),
  _directionalLightsColor(), _directionalLightsSize(0),
  _pointLightsPosition(),
  _pointLightsColor(), _pointLightsSize(0),
  _lightEnabled(false),
  _currentCamera(NULL), _programs()
{

}

gle::Scene::~Scene()
{
  // Delete created programs
  for (std::map<gle::Material*, gle::Program*>::iterator it = _programs.begin(), end = _programs.end();
       it != end; ++it)
    delete it->second;
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
  if (find(_meshes.begin(), _meshes.end(), mesh) == _meshes.end())
    _meshes.push_back(mesh);
  return (*this);
}

gle::Scene & gle::Scene::add(Material* material)
{
  if (find(_materials.begin(), _materials.end(), material) == _materials.end())
    _materials.push_back(material);
  return (*this);
}

gle::Scene & gle::Scene::add(Light* light)
{
  if (find(_lights.begin(), _lights.end(), light) == _lights.end())
    {
      _lights.push_back(light);
      this->updateLights();
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

std::map<gle::Material*, gle::Program*> & gle::Scene::getPrograms()
{
  return (_programs);
}

bool gle::Scene::isLightEnabled() const
{
  return (_lightEnabled);
}

void gle::Scene::setLightEnabled(bool enabled,
				 gle::Color<GLfloat> const& ambientColor)
{
  _lightEnabled = enabled;
  _ambientColor[0] = ambientColor.r;
  _ambientColor[1] = ambientColor.g;
  _ambientColor[2] = ambientColor.b;
}

GLfloat* gle::Scene::getAmbientColor()
{
  return (_ambientColor);
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
