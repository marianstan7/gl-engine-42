//
// Scene.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 19:12:49 2012 gael jochaud-du-plessix
// Last update Wed Jun  6 19:51:22 2012 gael jochaud-du-plessix
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
  _root.setName("root");
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

gle::Scene::Node & gle::Scene::getRootNode()
{
  return (_root);
}

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
  _pointLightsAttenuation.resize(0);
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
          GLfloat* attenuation = ((gle::PointLight*)(*it))->getAttenuation();
          _pointLightsPosition.push_back(position.x);
          _pointLightsPosition.push_back(position.y);
          _pointLightsPosition.push_back(position.z);
          _pointLightsColor.push_back(color[0]);
          _pointLightsColor.push_back(color[1]);
          _pointLightsColor.push_back(color[2]);
          _pointLightsSpecularColor.push_back(specularColor[0]);
          _pointLightsSpecularColor.push_back(specularColor[1]);
          _pointLightsSpecularColor.push_back(specularColor[2]);
	  _pointLightsAttenuation.push_back(attenuation[0]);
          _pointLightsAttenuation.push_back(attenuation[1]);
          _pointLightsAttenuation.push_back(attenuation[2]);
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

GLfloat* gle::Scene::getPointLightsAttenuation() const
{
  return ((GLfloat*)&_pointLightsAttenuation[0]);
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
      _program->getUniformLocation(gle::Program::PointLightAttenuation);
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
  shaderSource = _replace("%nb_point_lights", getPointLightsSize(), shaderSource);
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
  shaderSource = _replace("%nb_directional_lights", getDirectionalLightsSize(), shaderSource);
  shaderSource = _replace("%nb_point_lights", getPointLightsSize(), shaderSource);
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
  size_t pos = str.find(search);
  if (pos != std::string::npos)
    return (ret.replace(pos, search.size(), ss.str()));
  return (str);
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

void		gle::Scene::update(gle::Scene::Node* node, int depth)
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
  if (node->getType() == Node::Mesh && (mesh = dynamic_cast<Mesh*>(node)))
    {
      if (mesh->getBoundingVolume() && !mesh->isDynamic())
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
    update(child, depth + 1);
  if (generate && _frustumCulling)
    generateTree();
  if (generate)
    {
      _needProgramCompilation = true;
      updateLights();
      updateStaticMeshes();
    }
}

void gle::Scene::updateStaticMeshes()
{
  
}
