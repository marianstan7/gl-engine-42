//
// Scene.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 19:12:49 2012 gael jochaud-du-plessix
// Last update Wed May 30 01:30:05 2012 gael jochaud-du-plessix
//

#include <Scene.hpp>
#include <Mesh.hpp>
#include <Camera.hpp>
#include <algorithm>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <ShaderSource.hpp>
#include <sstream>

gle::Scene::Scene() :
  _backgroundColor(0.0, 0.0, 0.0, 0.0), _fogColor(0.0, 0.0, 0.0, 0.0), _fogDensity(0.0),
  _cameras(), _meshesToRender(), _unboundingMeshesToRender(),
  _lights(), _directionalLightsDirection(),
  _directionalLightsColor(), _directionalLightsSize(0),
  _pointLightsPosition(),
  _pointLightsColor(), _pointLightsSize(0),
  _currentCamera(NULL), _program(NULL), _needProgramCompilation(true),
  _displayBoundingVolume(false), _frustumCulling(false)
{
  //_root.setName("root");
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

void gle::Scene::setFogColor(gle::Color<GLfloat> const &color)
{
  _fogColor = color;
}

gle::Color<GLfloat> const & gle::Scene::getFogColor() const
{
  return (_fogColor);
}

void gle::Scene::setFogDensity(GLfloat density)
{
  _fogDensity = density;
}

GLfloat gle::Scene::getFogDensity() const
{
  return (_fogDensity);
}

gle::Scene & gle::Scene::add(gle::Scene::Node* node)
{
  _root.addChild(node);
  return (*this);
}

gle::Scene & gle::Scene::add(std::vector<gle::Scene::Node*> nodes)
{
  for (gle::Scene::Node* &node : nodes)
    _root.addChild(node);
  return (*this);
}

gle::Scene & gle::Scene::remove(gle::Scene::Node* node)
{
  _root.removeChild(node);
  return (*this);
}
/*
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
  if (mesh->getBoundingVolume() && find(_meshes.begin(), _meshes.end(), mesh) == _meshes.end())
    _meshes.push_back(mesh);
  else if (find(_meshes.begin(), _meshes.end(), mesh) == _meshes.end())
    _unboundingMeshes.push_back(mesh);
  std::vector<gle::Mesh*> & childs = mesh->getChildren();
  add(childs);
  if (_displayBoundingVolume && mesh->getBoundingVolume())
      add(mesh->getBoundingVolume()->getMesh());
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
  if ((it = find(_unboundingMeshes.begin(), _unboundingMeshes.end(), mesh)) != _unboundingMeshes.end())
    _unboundingMeshes.erase(it);
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
*/
const std::vector<gle::Mesh*> & gle::Scene::getMeshesToRender()
{
  if (_frustumCulling)
    return (reinterpret_cast<const std::vector<gle::Mesh*>&>
	    (_tree.getElementsInFrustum(_currentCamera->getProjectionMatrix(),
					_currentCamera->getTransformationMatrix())));
  return (_meshesToRender);
}

const std::vector<gle::Mesh*> & gle::Scene::getUnboundingMeshesToRender()
{
  return (_unboundingMeshesToRender);
}

std::vector<gle::Camera*> & gle::Scene::getCameras()
{
  return (_cameras);
}
/*
std::vector<gle::Material*> & gle::Scene::getMaterials()
{
  return (_materials);
}
*/
const std::vector<gle::Light*> & gle::Scene::getLights() const
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
  for (auto it = _lights.begin();
       it != _lights.end(); ++it)
    {
      if ((*it)->getLightType() == gle::Light::DIRECTIONAL)
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
      else if ((*it)->getLightType() == gle::Light::POINT)
	{
	  ++pSize;
	  gle::Vector3<GLfloat> position = ((gle::PointLight*)(*it))->getAbsolutePosition();
	  if (!_currentCamera)
	    throw (new gle::Exception::Exception("No camera for the scene..."));
	  position *= _currentCamera->getTransformationMatrix();
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
  _program->getUniformLocation(gle::Program::FogDensity);
  _program->getUniformLocation(gle::Program::FogColor);
  _program->getUniformLocation(gle::Program::HasColorMap);
  _program->getUniformLocation(gle::Program::ColorMap);
  if (getDirectionalLightsSize() || getPointLightsSize())
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
  gle::Shader *shader;
  try
    {
      shader = new gle::Shader(gle::Shader::Vertex, shaderSource);
    }
  catch (gle::Exception::CompilationError* e)
    {
      e->setFilename("Vertex.glsl");
      throw e;
    }

  return (shader);
}

gle::Shader* gle::Scene::_createFragmentShader()
{
  std::string shaderSource;

  shaderSource += gle::ShaderSource::FragmentShader;
  gle::Shader *shader;
  try
    {
      shader = new gle::Shader(gle::Shader::Fragment, shaderSource);
    }
  catch (gle::Exception::CompilationError* e)
    {
      e->setFilename("Fragment.glsl");
      throw e;
    }

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

void		gle::Scene::displayBoundingVolume()
{
  _displayBoundingVolume = true;
}

void		gle::Scene::generateTree()
{
  std::cout << "Starting octree generation..." << std::endl;
  _tree.generateTree(reinterpret_cast<std::vector<gle::Octree::Element*>&>(_meshesToRender));
  std::cout << "End of octree generation" << std::endl;
}

void		gle::Scene::displayTree()
{
  _tree.addMeshes(*this);
}

void		gle::Scene::enableFrustumCulling(bool enable)
{
  _frustumCulling = enable;
}

void		gle::Scene::setCamera(gle::Camera* camera)
{
  _currentCamera = camera;
}

void		gle::Scene::updateScene(gle::Scene::Node* node, int depth)
{
  Mesh*		mesh;
  Light*	light;
  Camera*	camera;
  bool		generate = false;

  if (!node)
    {
      _meshesToRender.clear();
      _lights.clear();
      _cameras.clear();
      _unboundingMeshesToRender.clear();
      node = &_root;
      generate = true;
      _root.updateMatrix();
    }
  //std::cout << depth << " : [" << node->getName() << std::endl;
  if (node->getType() == Node::Mesh && (mesh = dynamic_cast<Mesh*>(node)))
    {
      if (mesh->getBoundingVolume())
	_meshesToRender.push_back(mesh);
      else
	_unboundingMeshesToRender.push_back(mesh);
      if (_displayBoundingVolume && mesh->getBoundingVolume() &&
	  mesh->getBoundingVolume()->getMesh())
	_unboundingMeshesToRender.push_back(mesh->getBoundingVolume()->getMesh());
    }
  else if (node->getType() == Node::Light && (light = dynamic_cast<Light*>(node)))
    _lights.push_back(light);
  else if (! _currentCamera && node->getType() == Node::Camera && (camera = dynamic_cast<Camera*>(node)))
    _currentCamera = camera;
  const std::vector<Node*>& children = node->getChildren();
  for (Node* const &child : children)
    this->updateScene(child, depth + 1);
  if (generate && _frustumCulling)
    this->generateTree();
  if (generate)
    {
      _needProgramCompilation = true;
      this->updateLights();
    }
}
