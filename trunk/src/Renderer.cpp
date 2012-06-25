//
// Renderer.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 20:48:54 2012 gael jochaud-du-plessix
// Last update Mon Jun 25 20:31:07 2012 gael jochaud-du-plessix
//

#include <Renderer.hpp>
#include <gle/opengl.h>
#include <ShaderSource.hpp>
#include <Exception.hpp>
#include <EnvironmentMap.hpp>

gle::Renderer::Renderer() :
  _currentProgram(NULL),
  _indexesBuffer(gle::Bufferui::ElementArray,
		 gle::Bufferui::StaticDraw),
  _debugMode(0), _debugProgram(NULL)
{
  // Set color and depth clear value
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClearDepth(1.f);

  // Enable Z-buffer read and write 
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  // Backface culling
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);

  // Enable antialiasing
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

gle::Renderer::~Renderer()
{

}

void gle::Renderer::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gle::Renderer::render(Scene* scene, const Rectf& size, FrameBuffer* customFramebuffer)
{
  gle::FrameBuffer& framebuffer = customFramebuffer 
    ? *customFramebuffer : gle::FrameBuffer::getDefaultFrameBuffer();

  glViewport(size.x, size.y, size.width, size.height);

  framebuffer.bind();

  scene->processFrustumCulling();

  Camera*			camera = scene->getCurrentCamera();
  gle::Color<GLfloat> const &	backgroundColor = scene->getBackgroundColor();

  glClearColor(backgroundColor.r, backgroundColor.b, backgroundColor.a, 1.f);
  clear();
  if (scene->isEnvMapEnabled())
    _renderEnvMap(scene);
  _currentProgram = NULL;
  if (!camera)
    throw (new gle::Exception::Exception("No camera for the scene..."));

  _setCurrentProgram(scene, camera);

  const std::vector<gle::Mesh*> & staticMeshes = scene->getStaticMeshes();
  const std::vector<gle::Mesh*> & dynamicMeshes = scene->getDynamicMeshes();

  // Enable vertex attributes commons to all draws
  glEnableVertexAttribArray(gle::ShaderSource::PositionLocation);
  glEnableVertexAttribArray(gle::ShaderSource::NormalLocation);
  glEnableVertexAttribArray(gle::ShaderSource::TangentLocation);
  glEnableVertexAttribArray(gle::ShaderSource::MeshIdentifierLocation);

  MeshBufferManager::getInstance().bind();
  
  //Draw static meshes
  std::list<gle::Scene::MeshGroup> factorizedStaticMeshes =
    gle::Mesh::factorizeForDrawing(std::list<gle::Mesh*>(staticMeshes.begin(), staticMeshes.end()));

  //std::cout << "nb draw calls: " << factorizedStaticMeshes.size() << " for " << staticMeshes.size() << " meshes\n";

  for (gle::Scene::MeshGroup &group : factorizedStaticMeshes)
    {
      _buildIndexesBuffer(group.meshes);
      _renderMeshes(scene, group);
    }

  // Draw dynamic meshes
  for (gle::Mesh* mesh : dynamicMeshes)
    _renderMesh(scene, mesh);

  glDisableVertexAttribArray(gle::ShaderSource::PositionLocation);
  glDisableVertexAttribArray(gle::ShaderSource::NormalLocation);
  glDisableVertexAttribArray(gle::ShaderSource::TangentLocation);
  glDisableVertexAttribArray(gle::ShaderSource::MeshIdentifierLocation);
  glDisableVertexAttribArray(gle::ShaderSource::TextureCoordLocation);

  if (_debugMode)
    _renderDebugMeshes(scene);
  framebuffer.update();
}

void gle::Renderer::_buildIndexesBuffer(const std::list<gle::Mesh*> & meshes)
{
  GLuint	nbIndexes = 0;
  for (gle::Mesh* mesh : meshes)
    nbIndexes += mesh->getNbIndexes();

  _indexesBuffer.resize(nbIndexes);

  GLuint	i = 0;
  glBindBuffer(GL_COPY_WRITE_BUFFER, _indexesBuffer.getId());
  for (gle::Mesh* mesh : meshes)
    {
      gle::Bufferui* indexesBuffer = mesh->getIndexesBuffer();
      glBindBuffer(GL_COPY_READ_BUFFER, indexesBuffer->getId());
      glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
			  0, i * sizeof(GLuint), mesh->getNbIndexes() * sizeof(GLuint));
      i += mesh->getNbIndexes();
    }
}

void gle::Renderer::_renderEnvMap(gle::Scene* scene)
{
  
  _currentProgram = scene->getEnvMapProgram();
  _currentProgram->use();
  GLsizeiptr nbIndexes = scene->getEnvMapMesh()->getNbIndexes();
  gle::MeshBufferManager::Chunk* vertexAttributes = scene->getEnvMapMesh()->getAttributes();
  gle::Buffer<GLuint> * indexesBuffer = scene->getEnvMapMesh()->getIndexesBuffer();
  glEnableVertexAttribArray(gle::ShaderSource::PositionLocation);
  glVertexAttribPointer(gle::ShaderSource::PositionLocation,
                        3, GL_FLOAT, GL_FALSE,
                        gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
                        (GLvoid*)(vertexAttributes->getOffset() * sizeof(GLfloat)));
  gle::EnvironmentMap* envMap = scene->getEnvMap();
  if (envMap->getType() == EnvironmentMap::CubeMap)
    {
      glActiveTexture(gle::Program::CubeMapTexture);
      envMap->bind();
      _currentProgram->setUniform("gle_cubeMap",
				  gle::Program::CubeMapTextureIndex);
    }
  const gle::Matrix4<GLfloat>& mvMatrix = scene->getCurrentCamera()->getTransformationMatrix();
  _currentProgram->setUniform("gle_MVMatrix", mvMatrix);
  _currentProgram->setUniform("gle_PMatrix", scene->getCurrentCamera()->getProjectionMatrix());
  _currentProgram->setUniform("gle_CameraPos", scene->getCurrentCamera()->getPosition());
  indexesBuffer->bind();
  glPolygonMode(GL_FRONT_AND_BACK, scene->getEnvMapMesh()->getRasterizationMode());
  glDrawElements(scene->getEnvMapMesh()->getPrimitiveType(), nbIndexes, GL_UNSIGNED_INT, 0);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void gle::Renderer::_renderMeshes(gle::Scene* scene, gle::Scene::MeshGroup& group)
{
  scene->getStaticMeshesUniformsBuffer(group.uniformBufferId)->bindBase(gle::Program::StaticMeshesBlock);
  scene->getStaticMeshesMaterialsBuffer(group.materialBufferId)->bindBase(gle::Program::MaterialBlock);

  _setVertexAttributes(0);
  
  // Set up ColorMap
  if (group.colorMap || group.normalMap)
    glEnableVertexAttribArray(gle::ShaderSource::TextureCoordLocation);
  if (group.colorMap)
    {
      // Set texture to the shader
      glActiveTexture(gle::Program::ColorMapTexture);
      group.colorMap->bind();
      _currentProgram->setUniform("gle_colorMap",
      				  gle::Program::ColorMapTextureIndex);
    }
  if (group.normalMap)
    {
      // Set texture to the shader
      glActiveTexture(gle::Program::NormalMapTexture);
      group.normalMap->bind();
      _currentProgram->setUniform("gle_normalMap",
      				  gle::Program::NormalMapTextureIndex);
    }
  gle::Exception::CheckOpenGLError("Set uniform Normal map");

  // Set up EnvMap
  if (group.envMap)
    {
      if (group.envMap->getType() == EnvironmentMap::CubeMap)
	{
	  glActiveTexture(gle::Program::CubeMapTexture);
	  group.envMap->bind();
	  _currentProgram->setUniform("gle_cubeMap",
				      gle::Program::CubeMapTextureIndex);
	}
    }

  // Draw the mesh elements
  _indexesBuffer.bind();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gle::Exception::CheckOpenGLError("Before glDrawElements");
  glDrawElements(GL_TRIANGLES, _indexesBuffer.getSize(), GL_UNSIGNED_INT, 0);
  gle::Exception::CheckOpenGLError("glDrawElements");

  glDisableVertexAttribArray(gle::ShaderSource::TextureCoordLocation);  
}

void gle::Renderer::_renderMesh(gle::Scene* scene, gle::Mesh* mesh)
{
  GLsizeiptr nbIndexes = mesh->getNbIndexes();
  GLsizeiptr nbVertexes = mesh->getNbVertexes();

  if (nbIndexes < 1 || nbVertexes < 1)
    return ;

  gle::MeshBufferManager::Chunk* vertexAttributes = mesh->getAttributes();
  gle::Buffer<GLuint>* indexesBuffer = mesh->getIndexesBuffer();
  gle::Material* material = mesh->getMaterial();

  if (indexesBuffer == NULL || material == NULL || mesh == NULL ||
      !_currentProgram)
    return ;
  
  material->getUniformsBuffer()->bindBase(gle::Program::MaterialBlock);
  _currentProgram->setUniform("gle_MWMatrix", mesh->getTransformationMatrix());

  _setVertexAttributes(vertexAttributes->getOffset());

  // Set up ColorMap
  if (material->isColorMapEnabled() || material->isNormalMapEnabled())
    glEnableVertexAttribArray(gle::ShaderSource::TextureCoordLocation);

  if (material->isColorMapEnabled())
    {
      // Set texture to the shader
      gle::Texture* colorMap = material->getColorMap();
      glActiveTexture(gle::Program::ColorMapTexture);
      colorMap->bind();
      _currentProgram->setUniform("gle_colorMap",
      				  gle::Program::ColorMapTextureIndex);
    }
  gle::Exception::CheckOpenGLError("Set uniform Color map");
  if (material->isNormalMapEnabled())
    {
      // Set texture to the shader
      gle::Texture* normalMap = material->getNormalMap();
      glActiveTexture(gle::Program::NormalMapTexture);
      normalMap->bind();
      _currentProgram->setUniform("gle_normalMap",
      				  gle::Program::NormalMapTextureIndex);
    }
  gle::Exception::CheckOpenGLError("Set uniform Normal map");

  // Set up EnvMap
  if (material->isEnvMapEnabled())
    {
      gle::EnvironmentMap* envMap = material->getEnvMap();
      if (envMap->getType() == EnvironmentMap::CubeMap)
	{
	  glActiveTexture(gle::Program::CubeMapTexture);
	  envMap->bind();
	  _currentProgram->setUniform("gle_cubeMap",
				      gle::Program::CubeMapTextureIndex);
	}
    }

  // Draw the mesh elements
  indexesBuffer->bind();
  {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
      throw new gle::Exception::OpenGLError("_renderMesh: indexesBuffer->bind()");

  }

  if (mesh->getPrimitiveType() == gle::Mesh::Points
      || mesh->getRasterizationMode() == gle::Mesh::Point)
    glPointSize(mesh->getPointSize());
  glPolygonMode(GL_FRONT_AND_BACK, mesh->getRasterizationMode());
  gle::Exception::CheckOpenGLError("Before glDrawElements");
  glDrawElements(mesh->getPrimitiveType(), nbIndexes, GL_UNSIGNED_INT, 0);
  gle::Exception::CheckOpenGLError("glDrawElements");
  if (material->isColorMapEnabled())
    glDisableVertexAttribArray(gle::ShaderSource::TextureCoordLocation);
}

void gle::Renderer::_setVertexAttributes(GLuint offset)
{
  glVertexAttribPointer(gle::ShaderSource::PositionLocation,
			3, GL_FLOAT, GL_FALSE,
			gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)(offset * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::NormalLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::TangentLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords
				   + gle::Mesh::VertexAttributeSizeNormal)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::MeshIdentifierLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords
				   + gle::Mesh::VertexAttributeSizeNormal
				   + gle::Mesh::VertexAttributeSizeTangent
				   + gle::Mesh::VertexAttributeSizeTextureCoords)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::TextureCoordLocation,
			2, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords
				   + gle::Mesh::VertexAttributeSizeNormal
				   + gle::Mesh::VertexAttributeSizeTangent)
				  * sizeof(GLfloat)));
}

void gle::Renderer::_setCurrentProgram(gle::Scene* scene,
				       gle::Camera* camera)
{ 
  gle::Program* program = scene->getProgram();

  if (!program)
    throw (new gle::Exception::Exception("No program for the scene..."));
  if (program && program != _currentProgram)
    program->use();
  _currentProgram = program;
  _setSceneUniforms(scene, camera);
}

void gle::Renderer::_setSceneUniforms(gle::Scene* scene, gle::Camera* camera)
{
  const gle::Matrix4<GLfloat> & projectionMatrix = camera->getProjectionMatrix();

  _currentProgram->setUniform("gle_ViewMatrix", camera->getTransformationMatrix());
  _currentProgram->setUniform("gle_PMatrix", projectionMatrix);
  _currentProgram->setUniform("gle_CameraPos", camera->getAbsolutePosition());
  _currentProgram->setUniform("gle_fogColor", scene->getFogColor());
  _currentProgram->setUniform("gle_fogDensity", scene->getFogDensity());
  // Send light infos to the shader
  if (scene->getDirectionalLightsSize())
    {
      _currentProgram->setUniform3("gle_directionalLightDirection",
				    scene->getDirectionalLightsDirection(),
				    scene->getDirectionalLightsSize());
      _currentProgram->setUniform3("gle_directionalLightColor",
				    scene->getDirectionalLightsColor(),
				    scene->getDirectionalLightsSize());
    }
  if (scene->getPointLightsSize())
    {
      _currentProgram->setUniform3("gle_pointLightPosition",
      			    scene->getPointLightsPosition(),
      			    scene->getPointLightsSize());
      _currentProgram->setUniform3("gle_pointLightColor",
      			    scene->getPointLightsColor(),
      				    scene->getPointLightsSize());
      _currentProgram->setUniform3("gle_pointLightSpecularColor",
      			    scene->getPointLightsSpecularColor(),
      			    scene->getPointLightsSize());
      _currentProgram->setUniform3("gle_pointLightAttenuation",
      			    scene->getPointLightsAttenuation(),
      			    scene->getPointLightsSize());
    }
  if (scene->getSpotLightsSize())
    {
      _currentProgram->setUniform3("gle_spotLightPosition",
				    scene->getSpotLightsPosition(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3("gle_spotLightColor",
				    scene->getSpotLightsColor(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3("gle_spotLightSpecularColor",
				    scene->getSpotLightsSpecularColor(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3("gle_spotLightAttenuation",
				    scene->getSpotLightsAttenuation(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3("gle_spotLightDirection",
				    scene->getSpotLightsDirection(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform1("gle_spotLightCosCutOff",
				    scene->getSpotLightsCosCutOff(),
				    scene->getSpotLightsSize());
    }
}

void gle::Renderer::setDebugMode(int mode)
{
  _debugMode = mode;
}

void gle::Renderer::_renderDebugMeshes(gle::Scene* scene)
{  
  glClear(GL_DEPTH_BUFFER_BIT);
  if (!_debugProgram)
    {
      _debugProgram = new gle::Program();
      gle::Shader* vertexShader;
      gle::Shader* fragmentShader;
      vertexShader = new gle::Shader(gle::Shader::Vertex, gle::ShaderSource::DebugVertexShader);
      fragmentShader = new gle::Shader(gle::Shader::Fragment, gle::ShaderSource::DebugFragmentShader);
      _debugProgram->attach(*vertexShader);
      _debugProgram->attach(*fragmentShader);
      try {
        _debugProgram->link();
      }
      catch (std::exception *e)
        {
          delete vertexShader;
          delete fragmentShader;
          throw e;
        }
      _debugProgram->getUniformLocation("gle_MVMatrix");
      _debugProgram->getUniformLocation("gle_PMatrix");
      _debugProgram->getUniformLocation("gle_color");
    }
  _currentProgram = _debugProgram;
  _currentProgram->use();
  const std::vector<Scene::Node*>& debugNodes = scene->getDebugNodes(_debugMode);
  for (Scene::Node* const &debugNode : debugNodes)
  {
    Mesh* debugMesh = dynamic_cast<Mesh*>(debugNode);
    if (debugMesh)
      {
	GLsizeiptr nbIndexes = debugMesh->getNbIndexes();
	MeshBufferManager::Chunk* vertexAttributes = debugMesh->getAttributes();
	Buffer<GLuint> * indexesBuffer = debugMesh->getIndexesBuffer();
	glEnableVertexAttribArray(ShaderSource::PositionLocation);
	glVertexAttribPointer(ShaderSource::PositionLocation,
			      3, GL_FLOAT, GL_FALSE,
			      Mesh::VertexAttributesSize * sizeof(GLfloat),
			      (GLvoid*)(vertexAttributes->getOffset() * sizeof(GLfloat)));
	const Matrix4<GLfloat>& mvMatrix =  
	  scene->getCurrentCamera()->getTransformationMatrix() * debugMesh->getTransformationMatrix();
	_currentProgram->setUniform("gle_MVMatrix", mvMatrix);
	_currentProgram->setUniform("gle_PMatrix", scene->getCurrentCamera()->getProjectionMatrix());
	_currentProgram->setUniform("gle_color", debugMesh->getMaterial()->getAmbientColor());
	indexesBuffer->bind();
	glPolygonMode(GL_FRONT_AND_BACK, debugMesh->getRasterizationMode());
	glDrawElements(debugMesh->getPrimitiveType(), nbIndexes, GL_UNSIGNED_INT, 0);
      }
  }
}
