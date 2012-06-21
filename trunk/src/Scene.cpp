//
// Scene.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 19:12:49 2012 gael jochaud-du-plessix
// Last update Thu Jun 21 15:15:21 2012 gael jochaud-du-plessix
//

#include <Scene.hpp>
#include <Mesh.hpp>
#include <Camera.hpp>
#include <algorithm>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <SpotLight.hpp>
#include <ShaderSource.hpp>
#include <sstream>
#include <Geometries.hpp>

gle::Scene::Scene() :
  _backgroundColor(0.0, 0.0, 0.0, 0.0), _fogColor(0.0, 0.0, 0.0, 0.0), _fogDensity(0.0),
  _cameras(), _meshesToRender(), _unboundingMeshesToRender(),
  _lights(), _directionalLightsDirection(),
  _directionalLightsColor(), _directionalLightsSize(0),
  _pointLightsPosition(),
  _pointLightsColor(), _pointLightsSize(0),
  _currentCamera(NULL), _program(NULL), _needProgramCompilation(true),
  _staticMeshesUniformsBuffers(), _staticMeshesMaterialsBuffers(),
  _staticMeshesMaterialsBuffersIds(),
  _displayBoundingVolume(false), _frustumCulling(false),
  _envMap(NULL), _isEnvMapEnabled(false), _envMapProgram(NULL), _envMapMesh(NULL)
{
  _root.setName("root");
}

gle::Scene::~Scene()
{
  if (_envMapProgram)
    delete _envMapProgram;
  if (_envMapMesh)
    delete _envMapMesh;
  _clearStaticMeshesBuffers();
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
    return (_meshesInFrustum);
  return (_meshesToRender);
}

void gle::Scene::processFrustumCulling()
{
  if (_frustumCulling)
    _meshesInFrustum = reinterpret_cast<const std::vector<gle::Mesh*>&>
      (_tree.getElementsInFrustum(_currentCamera->getProjectionMatrix(),
				  _currentCamera->getTransformationMatrix()));
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

  _spotLightsPosition.resize(0);
  _spotLightsColor.resize(0);
  _spotLightsSpecularColor.resize(0);
  _spotLightsAttenuation.resize(0);
  _spotLightsDirection.resize(0);
  _spotLightsCosCutOff.resize(0);
  unsigned int dSize = 0;
  unsigned int pSize = 0;
  unsigned int sSize = 0;
  for (auto it = _lights.begin();
       it != _lights.end(); ++it)
    {
      if ((*it)->getLightType() == gle::Light::DIRECTIONAL)
	{
	  ++dSize;
	  gle::Vector3<GLfloat> direction = ((gle::DirectionalLight*)(*it))->getDirection();
	  if (!_currentCamera)
	    throw (new gle::Exception::Exception("No camera for the scene..."));
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
      else if ((*it)->getLightType() == gle::Light::SPOT)
	{
	  ++sSize;
	  gle::Vector3<GLfloat> position = ((gle::SpotLight*)(*it))->getAbsolutePosition();

	  if (!_currentCamera)
	    throw (new gle::Exception::Exception("No camera for the scene..."));
	  Vector3f direction = ((gle::SpotLight*)(*it))->getTarget();
	  direction *= _currentCamera->getTransformationMatrix();
	  position *= _currentCamera->getTransformationMatrix();
	  direction -= position;
          GLfloat* color = ((gle::SpotLight*)(*it))->getColor();
          GLfloat* specularColor = ((gle::SpotLight*)(*it))->getSpecularColor();
          GLfloat* attenuation = ((gle::SpotLight*)(*it))->getAttenuation();
          _spotLightsPosition.push_back(position.x);
          _spotLightsPosition.push_back(position.y);
          _spotLightsPosition.push_back(position.z);
          _spotLightsColor.push_back(color[0]);
          _spotLightsColor.push_back(color[1]);
          _spotLightsColor.push_back(color[2]);
          _spotLightsSpecularColor.push_back(specularColor[0]);
          _spotLightsSpecularColor.push_back(specularColor[1]);
          _spotLightsSpecularColor.push_back(specularColor[2]);
	  _spotLightsAttenuation.push_back(attenuation[0]);
          _spotLightsAttenuation.push_back(attenuation[1]);
          _spotLightsAttenuation.push_back(attenuation[2]);
	  _spotLightsDirection.push_back(direction.x);
          _spotLightsDirection.push_back(direction.y);
          _spotLightsDirection.push_back(direction.z);
          _spotLightsCosCutOff.push_back(((gle::SpotLight*)(*it))->getCosCutOff());
	}
    }
  _directionalLightsSize = dSize;
  _pointLightsSize = pSize;
  _spotLightsSize = sSize;
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

GLfloat* gle::Scene::getSpotLightsPosition() const
{
  return ((GLfloat*)&_spotLightsPosition[0]);
}

GLfloat* gle::Scene::getSpotLightsColor() const
{
  return ((GLfloat*)&_spotLightsColor[0]);
}

GLfloat* gle::Scene::getSpotLightsSpecularColor() const
{
  return ((GLfloat*)&_spotLightsSpecularColor[0]);
}

GLfloat* gle::Scene::getSpotLightsAttenuation() const
{
  return ((GLfloat*)&_spotLightsAttenuation[0]);
}

GLfloat* gle::Scene::getSpotLightsDirection() const
{
  return ((GLfloat*)&_spotLightsDirection[0]);
}

GLfloat* gle::Scene::getSpotLightsCosCutOff() const
{
  return ((GLfloat*)&_spotLightsCosCutOff[0]);
}

GLsizeiptr gle::Scene::getSpotLightsSize() const
{
  return (_spotLightsSize);
}

bool gle::Scene::hasLights() const
{
  return (getDirectionalLightsSize() || getPointLightsSize() || getSpotLightsSize());
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

  try
    {
      _program->link();
    }
  catch (std::exception *e)
    {
      delete vertexShader;
      delete fragmentShader;
      throw e;
    }

  //_program->getUniformLocation(gle::Program::MVMatrix);
  _program->getUniformLocation(gle::Program::ViewMatrix);
  _program->getUniformLocation(gle::Program::PMatrix);
  _program->getUniformLocation(gle::Program::CameraPos);
  _program->getUniformLocation(gle::Program::FogDensity);
  _program->getUniformLocation(gle::Program::FogColor);
  _program->getUniformLocation(gle::Program::ColorMap);
  if (getDirectionalLightsSize() || getPointLightsSize() || getSpotLightsSize())
    _program->getUniformLocation(gle::Program::NormalMap);
  _program->getUniformLocation(gle::Program::CubeMap);
  //if (getDirectionalLightsSize() || getPointLightsSize() || getSpotLightsSize())
  //_program->getUniformLocation(gle::Program::NMatrix);
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
  if (getSpotLightsSize())
    {
      _program->getUniformLocation(gle::Program::SpotLightPosition);
      _program->getUniformLocation(gle::Program::SpotLightColor);
      _program->getUniformLocation(gle::Program::SpotLightSpecularColor);
      _program->getUniformLocation(gle::Program::SpotLightAttenuation);
      _program->getUniformLocation(gle::Program::SpotLightDirection);
      _program->getUniformLocation(gle::Program::SpotLightCosCutOff);
    }
  _program->retreiveUniformBlockIndex(gle::Program::MaterialBlock, "materialBlock");
  _program->retreiveUniformBlockIndex(gle::Program::StaticMeshesBlock, "staticMeshesBlock");
  delete vertexShader;
  delete fragmentShader;
}

gle::Shader* gle::Scene::_createVertexShader()
{
  std::string shaderSource;
  GLint	maxUniformBlockSize = -1, maxMeshByBuffer = 0, maxMaterialByBuffer = 0;
  
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  maxMeshByBuffer = maxUniformBlockSize / (MeshUniformSize * sizeof(GLfloat));
  maxMaterialByBuffer = maxUniformBlockSize / (gle::Material::UniformSize * sizeof(GLfloat));

  shaderSource += gle::ShaderSource::VertexShader;
  shaderSource = _replace("%nb_directional_lights", getDirectionalLightsSize(), shaderSource);
  shaderSource = _replace("%nb_point_lights", getPointLightsSize(), shaderSource);
  shaderSource = _replace("%nb_spot_lights", getSpotLightsSize(), shaderSource);
  shaderSource = _replace("%nb_static_meshes", maxMeshByBuffer, shaderSource);
  shaderSource = _replace("%nb_materials", maxMaterialByBuffer, shaderSource);

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

  GLint	maxUniformBlockSize = -1, maxMeshByBuffer = 0, maxMaterialByBuffer = 0;
  
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  maxMeshByBuffer = maxUniformBlockSize / (MeshUniformSize * sizeof(GLfloat));
  maxMaterialByBuffer = maxUniformBlockSize / (gle::Material::UniformSize * sizeof(GLfloat));

  shaderSource += gle::ShaderSource::FragmentShader;
  shaderSource = _replace("%nb_directional_lights", getDirectionalLightsSize(), shaderSource);
  shaderSource = _replace("%nb_point_lights", getPointLightsSize(), shaderSource);
  shaderSource = _replace("%nb_spot_lights", getSpotLightsSize(), shaderSource);
  shaderSource = _replace("%nb_static_meshes", maxMeshByBuffer, shaderSource);
  shaderSource = _replace("%nb_materials", maxMaterialByBuffer, shaderSource);

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
  GLint	maxUniformBlockSize = -1, maxMeshByBuffer = 0;
  
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  maxMeshByBuffer = maxUniformBlockSize / (MeshUniformSize * sizeof(GLfloat));

  _clearStaticMeshesBuffers();
  _staticMeshesUniformsBuffers.reserve(_meshesToRender.size() / maxMeshByBuffer);

  GLfloat* staticMeshesUniforms = new GLfloat[MeshUniformSize * maxMeshByBuffer];

  std::list<MeshGroup> factorizedMeshes =
    gle::Mesh::factorizeForDrawing(std::list<gle::Mesh*>(_meshesToRender.begin(), _meshesToRender.end()),
				   true);

  _buildMaterialBuffers(factorizedMeshes, maxUniformBlockSize);

  int bufferId = 0;

  for (MeshGroup &group : factorizedMeshes)
    {
      while (group.meshes.size() > 0)
	{
	  gle::Bufferf* buffer = new gle::Bufferf(gle::Bufferf::UniformArray);
	  int nbMeshes = group.meshes.size();
	  if (nbMeshes > maxMeshByBuffer)
	    nbMeshes = maxMeshByBuffer;
	  for (int i = 0; i < nbMeshes; ++i)
	    {
	      gle::Mesh* mesh = group.meshes.front();
	      group.meshes.pop_front();
	      mesh->setUniformBufferId(bufferId);
	      mesh->setMaterialBufferId(_staticMeshesMaterialsBuffersIds[mesh->getMaterial()].first);
	      mesh->setIdentifiers(i, _staticMeshesMaterialsBuffersIds[mesh->getMaterial()].second);
	      const gle::Matrix4<GLfloat>& mvMatrix = mesh->getTransformationMatrix();

	      for (int j = 0; j < MeshUniformSize; ++j)
		staticMeshesUniforms[MeshUniformSize * i + j] = ((const GLfloat*)mvMatrix)[j];
	    }
	  buffer->resize(nbMeshes * MeshUniformSize, staticMeshesUniforms);
	  _staticMeshesUniformsBuffers.resize(bufferId + 1, NULL);
	  _staticMeshesUniformsBuffers[bufferId] = buffer;
	  ++bufferId;
	}
    }

  delete[] staticMeshesUniforms;
}

void gle::Scene::setEnvMap(EnvironmentMap* envMap)
{
  _envMap = envMap;
  _isEnvMapEnabled = true;
  if (!_envMapProgram)
    {
      _envMapProgram = new gle::Program();
      
      gle::Shader* vertexShader;
      gle::Shader* fragmentShader;
      try
	{
	  vertexShader = new gle::Shader(gle::Shader::Vertex, "#version 330 core\n"
					 "#define GLE_IN_VERTEX_POSITION_LOCATION 0\n"
					 "layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition;\n"
					 "uniform mat4 gle_MVMatrix;\n"
					 "uniform mat4 gle_PMatrix;\n"
					 "uniform vec3 gle_CameraPos;\n"
					 "out vec3 pos;\n"
					 "void main(void) {\n"
					 "vec4 gle_mvPosition = gle_MVMatrix * vec4(gle_vPosition + gle_CameraPos, 1);\n"
					 "gl_Position = (gle_PMatrix * gle_mvPosition);\n"
					 "pos = gle_vPosition;\n"
					 "}\n");
	}
      catch (gle::Exception::CompilationError* e)
	{
	  e->setFilename("EnvMap Vertex Shader");
	  throw e;
	}
      try
	{
	  fragmentShader = new gle::Shader(gle::Shader::Fragment, "#version 330 core\n"
					   "#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0\n"
					   "layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor;\n"
					   "uniform samplerCube gle_cubeMap;"
					   "in vec3 pos;\n"
					   "void main(void) {\n"
					   "gle_FragColor = vec4(texture(gle_cubeMap, pos).rgb, 1.0);"
					   "}\n");
	}
      catch (gle::Exception::CompilationError* e)
	{
	  e->setFilename("EnvMap Fragment Shader");
	  throw e;
	}
      
      _envMapProgram->attach(*vertexShader);
      _envMapProgram->attach(*fragmentShader);
      
      try {
	_envMapProgram->link();
      }
      catch (std::exception *e)
	{
	  delete vertexShader;
	  delete fragmentShader;
	  throw e;
	}
      _envMapProgram->getUniformLocation(Program::CameraPos);
      _envMapProgram->getUniformLocation(Program::MVMatrix);
      _envMapProgram->getUniformLocation(Program::PMatrix);
      _envMapProgram->getUniformLocation(Program::CubeMap);
    }
  if (!_envMapMesh)
    {
      _envMapMesh = Geometries::Cube(NULL, 1000);
    }
}

void gle::Scene::setEnvMapEnabled(bool enabled)
{
  _isEnvMapEnabled = enabled;
}

gle::EnvironmentMap* gle::Scene::getEnvMap() const
{
  return (_envMap);
}

bool gle::Scene::isEnvMapEnabled() const
{
  return (_isEnvMapEnabled);
}

gle::Program* gle::Scene::getEnvMapProgram() const
{
  return (_envMapProgram);
}

gle::Mesh* gle::Scene::getEnvMapMesh() const
{
  return (_envMapMesh);
}

void	gle::Scene::_buildMaterialBuffers(std::list<MeshGroup>& factorizedMeshes, GLint maxUniformBlockSize)
{
  GLint	maxMaterialByBuffer = 0;

  maxMaterialByBuffer = maxUniformBlockSize / (gle::Material::UniformSize * sizeof(GLfloat));

  GLfloat* staticMeshesMaterials = new GLfloat[gle::Material::UniformSize * maxMaterialByBuffer];
  int bufferId = 0;

  for (MeshGroup &group : factorizedMeshes)
    {
      std::list<gle::Material*> materials;
      for (gle::Mesh* mesh : group.meshes)
	{
	  gle::Material* material = mesh->getMaterial();
	  if (find(materials.begin(), materials.end(), material) == materials.end())
	    materials.push_front(material);
	}

      while (materials.size() > 0)
      	{
      	  gle::Bufferf* buffer = new gle::Bufferf(gle::Bufferf::UniformArray);
	  int nbMaterials = materials.size();
	  if (nbMaterials > maxMaterialByBuffer)
	    nbMaterials = maxMaterialByBuffer;
	  for (int i = 0; i < nbMaterials; ++i)
	    {
	      gle::Material* material = materials.front();
	      materials.pop_front();
	      _staticMeshesMaterialsBuffersIds[material] = {bufferId, i};
	      const GLfloat* materialData = material->getUniforms();
	      for (int j = 0; j < gle::Material::UniformSize; ++j)
	      	staticMeshesMaterials[gle::Material::UniformSize * i + j] = materialData[j];
	    }
	  buffer->resize(nbMaterials * gle::Material::UniformSize, staticMeshesMaterials);
	  _staticMeshesMaterialsBuffers.resize(bufferId + 1, NULL);
	  _staticMeshesMaterialsBuffers[bufferId] = buffer;
	  ++bufferId;
      	}
    }

  delete[] staticMeshesMaterials;
}

void	gle::Scene::_clearStaticMeshesBuffers()
{
  for (gle::Bufferf* buffer : _staticMeshesUniformsBuffers)
    delete buffer;
  for (gle::Bufferf* buffer : _staticMeshesMaterialsBuffers)
    delete buffer;
  _staticMeshesUniformsBuffers.clear();
  _staticMeshesMaterialsBuffers.clear();
  _staticMeshesMaterialsBuffersIds.clear();
}

const gle::Bufferf*	gle::Scene::getStaticMeshesUniformsBuffer(GLuint bufferId) const
{
  return (_staticMeshesUniformsBuffers[bufferId]);
}

const gle::Bufferf*	gle::Scene::getStaticMeshesMaterialsBuffer(GLuint bufferId) const
{
  return (_staticMeshesMaterialsBuffers[bufferId]);
}
