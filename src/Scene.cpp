//
// Scene.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Thu Jun 21 20:42:49 2012 loick michard
// Last update Wed Jul  4 19:51:51 2012 gael jochaud-du-plessix
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
#include <Renderer.hpp>
#include <Bone.hpp>
#include <Skeleton.hpp>

gle::Scene::Scene() :
  _backgroundColor(0.0, 0.0, 0.0, 0.0), _fogColor(0.0, 0.0, 0.0, 0.0), _fogDensity(0.0),
  _cameras(), _staticMeshes(), _dynamicMeshes(),
  _lights(), _directionalLightsDirection(),
  _directionalLightsColor(), _directionalLightsSize(0),
  _pointLightsPosition(),
  _pointLightsColor(), _pointLightsSize(0),
  _spotLightsSize(0),
  _currentCamera(NULL), _program(NULL), _needProgramCompilation(true),
  _staticMeshesUniformsBuffers(), _staticMeshesMaterialsBuffers(),
  _staticMeshesMaterialsBuffersIds(),
  _frustumCulling(false),
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

gle::Scene & gle::Scene::add(std::vector<Node*> nodes)
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

const std::list<gle::Mesh*> & gle::Scene::getStaticMeshes()
{
  if (_frustumCulling)
    return (_meshesInFrustum);
  return (_staticMeshes);
}

const std::list<gle::Mesh*> & gle::Scene::getDynamicMeshes()
{
  return (_dynamicMeshes);
}

void gle::Scene::processFrustumCulling()
{
  if (_frustumCulling)
    _meshesInFrustum = reinterpret_cast<const std::list<gle::Mesh*>&>
      (_tree.getElementsInFrustum(_currentCamera->getProjectionMatrix(),
				  _currentCamera->getTransformationMatrix()));
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

void gle::Scene::updateShadowMaps(gle::Renderer* renderer)
{
  for (gle::Light* light : _lights)
    updateShadowMap(renderer, light);
}

void gle::Scene::updateShadowMap(gle::Renderer* renderer, gle::Light* light)
{
  gle::Texture*		shadowMap;
  gle::FrameBuffer*	frameBuffer;
  gle::Camera*		lightCamera;
  if (!light->projectShadow()
      || !(lightCamera = light->getShadowMapCamera())
      || !(shadowMap = light->getShadowMap())
      || !(frameBuffer = light->getShadowMapFrameBuffer()))
    return ;
  std::list<gle::Mesh*> staticMeshes;
  if (_frustumCulling)
    staticMeshes = reinterpret_cast<const std::list<gle::Mesh*>&>
      (_tree.getElementsInFrustum(lightCamera->getProjectionMatrix(),
				  lightCamera->getTransformationMatrix()));
  else
    staticMeshes = _staticMeshes;
  for (auto it = staticMeshes.begin(); it != staticMeshes.end();)
    {
      if (!(*it)->projectShadow())
	it = staticMeshes.erase(it);
      else
	++it;
    }
  std::list<gle::Mesh*> dynamicMeshes = getDynamicMeshes();
  for (auto it = dynamicMeshes.begin(); it != dynamicMeshes.end();)
    {
      if (!(*it)->projectShadow())
	it = dynamicMeshes.erase(it);
      else
	++it;
    }
  renderer->renderShadowMap(this, staticMeshes, dynamicMeshes, light);
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
  _spotLightsHasShadowMap.resize(0);
  _spotLightsShadowMapMatrix.resize(0);
  _spotLightsShadowMap.resize(0);

  gle::Matrix4f
    shadowMapBiasMatrix(
			0.5, 0.0, 0.0, 0.5,
			0.0, 0.5, 0.0, 0.5,
			0.0, 0.0, 0.5, 0.5,
			0.0, 0.0, 0.0, 1.0
			);

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
	  gle::SpotLight* light = dynamic_cast<gle::SpotLight*>(*it);

	  gle::Vector3<GLfloat> position = light->getAbsolutePosition();

	  if (!_currentCamera)
	    throw (new gle::Exception::Exception("No camera for the scene..."));
	  Vector3f direction = light->getTarget();
	  direction *= _currentCamera->getTransformationMatrix();
	  position *= _currentCamera->getTransformationMatrix();
	  direction -= position;
          GLfloat* color = light->getColor();
          GLfloat* specularColor = light->getSpecularColor();
          GLfloat* attenuation = light->getAttenuation();
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
          _spotLightsCosCutOff.push_back(light->getCosCutOff());

	  if (light->projectShadow()
	      && light->getShadowMapCamera()
	      && light->getShadowMap())
	    {
	      _spotLightsHasShadowMap.push_back(true);
	      gle::Matrix4f VPMatrix =
		shadowMapBiasMatrix
		* light->getShadowMapCamera()->getProjectionMatrix()
		* light->getShadowMapCamera()->getTransformationMatrix();
	      GLfloat* VPMatrixf = (GLfloat*)VPMatrix;
	      for (int i = 0; i < 16; ++i)
		_spotLightsShadowMapMatrix.push_back(VPMatrixf[i]);
	      _spotLightsShadowMap.push_back(light->getShadowMap());
	    }
	  else
	    {
	      _spotLightsHasShadowMap.push_back(false);
	      for (int i = 0; i < 16; ++i)
		_spotLightsShadowMapMatrix.push_back(0);
	      _spotLightsShadowMap.push_back(NULL);
	    }
	}
    }
  _directionalLightsSize = dSize;
  _pointLightsSize = pSize;
  _spotLightsSize = sSize;
}

void gle::Scene::updateSkeletons()
{
  _bonesMatrices.clear();
  for (Skeleton* &skeleton : _skeletons)
    {
      skeleton->setId(_bonesMatrices.size() / 16);
      updateSkeleton(skeleton);
    }
} 

void gle::Scene::updateSkeleton(Scene::Node* node)
{
  if (!node)
    return;
  if (node->getType() == Node::Bone)
    {
      Matrix4<GLfloat> matrix = node->getTransformationMatrix();
      GLfloat* floatMatrix = (GLfloat*)matrix;
      for (int i = 0; i < 16; ++i)
        _bonesMatrices.push_back(floatMatrix[i]);
    }
  const std::vector<Node*>& children = node->getChildren();
  for (Node* const &child : children)
    updateSkeleton(child);
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

GLint*	gle::Scene::getSpotLightsHasShadowMap() const
{
  return ((GLint*)&_spotLightsHasShadowMap[0]);
}

GLfloat* gle::Scene::getSpotLightsShadowMapMatrix() const
{
  return ((GLfloat*)&_spotLightsShadowMapMatrix[0]);
}

const std::vector<gle::Texture*>& gle::Scene::getSpotLightsShadowMap() const
{
  return (_spotLightsShadowMap);
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

  _program->getUniformLocation("gle_MWMatrix");
  _program->getUniformLocation("gle_ViewMatrix");
  _program->getUniformLocation("gle_PMatrix");
  _program->getUniformLocation("gle_CameraPos");
  _program->getUniformLocation("gle_fogDensity");
  _program->getUniformLocation("gle_fogColor");
  _program->getUniformLocation("gle_colorMap");
  if (getDirectionalLightsSize() || getPointLightsSize() || getSpotLightsSize())
    _program->getUniformLocation("gle_normalMap");
  _program->getUniformLocation("gle_cubeMap");
  //if (getDirectionalLightsSize() || getPointLightsSize() || getSpotLightsSize())
  //_program->getUniformLocation(gle::Program::NMatrix);
  if (_bonesMatrices.size())
    _program->getUniformLocation("gle_bonesMatrix");
  if (getDirectionalLightsSize())
    {
      _program->getUniformLocation("gle_directionalLightDirection");
      _program->getUniformLocation("gle_directionalLightColor");
    }
  if (getPointLightsSize())
    {
      _program->getUniformLocation("gle_pointLightPosition");
      _program->getUniformLocation("gle_pointLightColor");
      _program->getUniformLocation("gle_pointLightSpecularColor");
      _program->getUniformLocation("gle_pointLightAttenuation");
    }
  if (getSpotLightsSize())
    {
      _program->getUniformLocation("gle_spotLightPosition");
      _program->getUniformLocation("gle_spotLightColor");
      _program->getUniformLocation("gle_spotLightSpecularColor");
      _program->getUniformLocation("gle_spotLightAttenuation");
      _program->getUniformLocation("gle_spotLightDirection");
      _program->getUniformLocation("gle_spotLightCosCutOff");
      _program->getUniformLocation("gle_spotLightHasShadowMap");
      _program->getUniformLocation("gle_spotLightShadowMapMatrix");
      _program->getUniformLocation("gle_spotLightShadowMap");
    }
  _program->retreiveUniformBlockIndex("gle_materialBlock");
  _program->retreiveUniformBlockIndex("gle_staticMeshesBlock");
  delete vertexShader;
  delete fragmentShader;
}

void	gle::Scene::setShaderSourceConstants(std::string& shaderSource)
{

  GLint	maxUniformBlockSize = -1, maxMeshByBuffer = 0, maxMaterialByBuffer = 0;
  
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  maxMeshByBuffer = maxUniformBlockSize / (Mesh::UniformSize * sizeof(GLfloat));
  maxMaterialByBuffer = maxUniformBlockSize / (gle::Material::UniformSize * sizeof(GLfloat));

  shaderSource = _replace("%nb_directional_lights", getDirectionalLightsSize(), shaderSource);
  shaderSource = _replace("%nb_point_lights", getPointLightsSize(), shaderSource);
  shaderSource = _replace("%nb_spot_lights", getSpotLightsSize(), shaderSource);
  shaderSource = _replace("%nb_static_meshes", maxMeshByBuffer, shaderSource);
  shaderSource = _replace("%nb_materials", maxMaterialByBuffer, shaderSource);
  shaderSource = _replace("%nb_bones", _bonesMatrices.size() / 16, shaderSource);
}

gle::Shader* gle::Scene::_createVertexShader()
{
  std::string shaderSource;

  shaderSource += gle::ShaderSource::VertexShader;
  setShaderSourceConstants(shaderSource);

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
  setShaderSourceConstants(shaderSource);

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

void		gle::Scene::generateTree()
{
  std::cout << "Starting octree generation..." << std::endl;
  _tree.generateTree(reinterpret_cast<std::list<gle::Octree::Element*>&>(_staticMeshes));
  std::cout << "End of octree generation" << std::endl;
}

void		gle::Scene::enableFrustumCulling(bool enable)
{
  _frustumCulling = enable;
}

void		gle::Scene::setCurrentCamera(gle::Camera* camera)
{
  _currentCamera = camera;
}

void		gle::Scene::update(gle::Scene::Node* node, int depth)
{
  Mesh*		mesh;
  Light*	light;
  Camera*	camera;
  bool		generate = false;
  GLsizeiptr	directionalLightsSize = _directionalLightsSize;
  GLsizeiptr	pointLightsSize = _pointLightsSize;
  GLsizeiptr	spotLightsSize = _spotLightsSize;
  uint		bonesSize = _bonesMatrices.size() / 16;

  if (!node)
    {
      _staticMeshes.clear();
      _dynamicMeshes.clear();
      _lights.clear();
      _cameras.clear();
      _skeletons.clear();
      node = &_root;
      generate = true;
      _root.updateMatrix();
    }
  if (node->getType() == Node::Skeleton)
    _skeletons.push_back(dynamic_cast<gle::Skeleton*>(node));
  else if (node->getType() == Node::Mesh && (mesh = dynamic_cast<Mesh*>(node)))
    {
      if (mesh->getBoundingVolume() && !mesh->isDynamic())
	_staticMeshes.push_back(mesh);
      else
	_dynamicMeshes.push_back(mesh);
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
      updateLights();
      updateSkeletons();
      updateDynamicMeshes();
      updateStaticMeshes();
      if (directionalLightsSize != _directionalLightsSize ||
      	  pointLightsSize != _pointLightsSize ||
      	  spotLightsSize != _spotLightsSize ||
      	  bonesSize != _bonesMatrices.size() / 16)
      	_needProgramCompilation = true;
    }
}

void gle::Scene::updateDynamicMeshes()
{
  for (gle::Mesh* mesh : _dynamicMeshes)
    mesh->setIdentifiers(0, 0);
}

void gle::Scene::updateStaticMeshes()
{
  GLint	maxUniformBlockSize = -1, maxMeshByBuffer = 0;
 
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  maxMeshByBuffer = maxUniformBlockSize / (Mesh::UniformSize * sizeof(GLfloat));

  _clearStaticMeshesBuffers();
  _staticMeshesUniformsBuffers.reserve(_staticMeshes.size() / maxMeshByBuffer);

  GLfloat* staticMeshesUniforms = new GLfloat[Mesh::UniformSize * maxMeshByBuffer];

  std::list<MeshGroup> factorizedMeshes =
    gle::Mesh::factorizeForDrawing(_staticMeshes, true);

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
	      const GLfloat* buffer = mesh->getUniforms();

	      for (int j = 0; j < Mesh::UniformSize; ++j)
		staticMeshesUniforms[Mesh::UniformSize * i + j] = buffer[j];
	    }
	  buffer->resize(nbMeshes * Mesh::UniformSize, staticMeshesUniforms);
	  _staticMeshesUniformsBuffers.resize(bufferId + 1, NULL);
	  _staticMeshesUniformsBuffers[bufferId] = buffer;
	  ++bufferId;
	}
    }

  delete[] staticMeshesUniforms;
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
	  vertexShader = new gle::Shader(gle::Shader::Vertex, gle::ShaderSource::CubeMapVertexShader);
	}
      catch (gle::Exception::CompilationError* e)
	{
	  e->setFilename("EnvMap Vertex Shader");
	  throw e;
	}
      try
	{
	  fragmentShader = new gle::Shader(gle::Shader::Fragment, gle::ShaderSource::CubeMapFragmentShader);
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
      _envMapProgram->getUniformLocation("gle_CameraPos");
      _envMapProgram->getUniformLocation("gle_MVMatrix");
      _envMapProgram->getUniformLocation("gle_PMatrix");
      _envMapProgram->getUniformLocation("gle_cubeMap");
    }
  if (!_envMapMesh)
    _envMapMesh = Geometries::Cube(NULL, 100, true);
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

std::vector<gle::Scene::Node*>& gle::Scene::getDebugNodes(int mode)
{
  _debugNodes.clear();
  _addDebugNodes(&_root, mode);
  if (mode & Renderer::Octree)
    {
      const std::vector<Mesh*>& debugMeshes = _tree.getDebugNodes();
      for (Mesh* const& debugMesh : debugMeshes)
	_debugNodes.push_back(debugMesh);
    }
  return (_debugNodes);
}

void gle::Scene::_addDebugNodes(Scene::Node* node, int mode)
{
  if (node)
    {
      const std::vector<Scene::Node*>& debugNodes = node->getDebugNodes(mode);
      for (Scene::Node* const &debugNode : debugNodes)
	_debugNodes.push_back(debugNode);
      const std::vector<Node*>& children = node->getChildren();
      for (Node* const &child : children)
	_addDebugNodes(child, mode);
    }
}

std::vector<GLfloat>& gle::Scene::getBones()
{
  return (_bonesMatrices);
}
