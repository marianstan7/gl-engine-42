//
// Renderer.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 20:48:54 2012 gael jochaud-du-plessix
// Last update Fri Jun 22 00:01:55 2012 gael jochaud-du-plessix
//

#include <Renderer.hpp>
#include <gle/opengl.h>
#include <ShaderSource.hpp>
#include <Exception.hpp>
#include <EnvironmentMap.hpp>

gle::Renderer::Renderer() :
  _currentProgram(NULL),
  _indexesBuffer(gle::Bufferui::ElementArray,
		 gle::Bufferui::StaticDraw)
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
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::PositionLocation);
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::NormalLocation);
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::TangentLocation);
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::MeshIdentifierLocation);

  MeshBufferManager::getInstance().bind();
  
  //Draw static meshes
  std::list<gle::Scene::MeshGroup> factorizedStaticMeshes =
    gle::Mesh::factorizeForDrawing(std::list<gle::Mesh*>(staticMeshes.begin(), staticMeshes.end()));
  for (gle::Scene::MeshGroup &group : factorizedStaticMeshes)
    {
      _buildIndexesBuffer(group.meshes);
      _renderMeshes(scene, group);
    }

  // Draw dynamic meshes
  for (gle::Mesh* mesh : dynamicMeshes)
    _renderMesh(scene, mesh);

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
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::PositionLocation);
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::PositionLocation,
                        3, GL_FLOAT, GL_FALSE,
                        gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
                        (GLvoid*)(vertexAttributes->getOffset() * sizeof(GLfloat)));
  gle::EnvironmentMap* envMap = scene->getEnvMap();
  if (envMap->getType() == EnvironmentMap::CubeMap)
    {
      glActiveTexture(gle::Program::CubeMapTexture);
      envMap->bind();
      _currentProgram->setUniform(gle::Program::CubeMap,
				  gle::Program::CubeMapTextureIndex);
    }
  const gle::Matrix4<GLfloat>& mvMatrix = scene->getCurrentCamera()->getTransformationMatrix();
  _currentProgram->setUniform(gle::Program::MVMatrix, mvMatrix);
  _currentProgram->setUniform(gle::Program::PMatrix, scene->getCurrentCamera()->getProjectionMatrix());
  _currentProgram->setUniform(gle::Program::CameraPos, scene->getCurrentCamera()->getPosition());
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
    glEnableVertexAttribArray(gle::ShaderSource::Vertex::ColorMap::TextureCoordLocation);
  if (group.colorMap)
    {
      // Set texture to the shader
      glActiveTexture(gle::Program::ColorMapTexture);
      group.colorMap->bind();
      _currentProgram->setUniform(gle::Program::ColorMap,
      				  gle::Program::ColorMapTextureIndex);
    }
  if (group.normalMap)
    {
      // Set texture to the shader
      glActiveTexture(gle::Program::NormalMapTexture);
      group.normalMap->bind();
      _currentProgram->setUniform(gle::Program::NormalMap,
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
	  _currentProgram->setUniform(gle::Program::CubeMap,
				      gle::Program::CubeMapTextureIndex);
	}
    }

  // Draw the mesh elements
  _indexesBuffer.bind();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gle::Exception::CheckOpenGLError("Before glDrawElements");
  glDrawElements(GL_TRIANGLES, _indexesBuffer.getSize(), GL_UNSIGNED_INT, 0);
  gle::Exception::CheckOpenGLError("glDrawElements");

  glDisableVertexAttribArray(gle::ShaderSource::Vertex::ColorMap::TextureCoordLocation);  
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
  _currentProgram->setUniform(gle::Program::MWMatrix, mesh->getTransformationMatrix());

  _setVertexAttributes(vertexAttributes->getOffset());

  // Set up ColorMap
  if (material->isColorMapEnabled() || material->isNormalMapEnabled())
    glEnableVertexAttribArray(gle::ShaderSource::Vertex::ColorMap::TextureCoordLocation);
  if (material->isColorMapEnabled())
    {
      // Set texture to the shader
      gle::Texture* colorMap = material->getColorMap();
      glActiveTexture(gle::Program::ColorMapTexture);
      colorMap->bind();
      _currentProgram->setUniform(gle::Program::ColorMap,
      				  gle::Program::ColorMapTextureIndex);
    }
  gle::Exception::CheckOpenGLError("Set uniform Color map");
  if (material->isNormalMapEnabled())
    {
      // Set texture to the shader
      gle::Texture* normalMap = material->getNormalMap();
      glActiveTexture(gle::Program::NormalMapTexture);
      normalMap->bind();
      _currentProgram->setUniform(gle::Program::NormalMap,
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
	  _currentProgram->setUniform(gle::Program::CubeMap,
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
    glDisableVertexAttribArray(gle::ShaderSource::Vertex::
			       ColorMap::TextureCoordLocation);
}

void gle::Renderer::_setVertexAttributes(GLuint offset)
{
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::PositionLocation,
			3, GL_FLOAT, GL_FALSE,
			gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)(offset * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::NormalLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::TangentLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords
				   + gle::Mesh::VertexAttributeSizeNormal)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::MeshIdentifierLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((offset
				   + gle::Mesh::VertexAttributeSizeCoords
				   + gle::Mesh::VertexAttributeSizeNormal
				   + gle::Mesh::VertexAttributeSizeTangent
				   + gle::Mesh::VertexAttributeSizeTextureCoords)
				  * sizeof(GLfloat)));
  glVertexAttribPointer(gle::ShaderSource::Vertex::
			ColorMap::TextureCoordLocation,
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

  _currentProgram->setUniform(gle::Program::ViewMatrix, camera->getTransformationMatrix());
  _currentProgram->setUniform(gle::Program::PMatrix, projectionMatrix);
  _currentProgram->setUniform(gle::Program::CameraPos, camera->getAbsolutePosition());
  _currentProgram->setUniform(gle::Program::FogColor, scene->getFogColor());
  _currentProgram->setUniform(gle::Program::FogDensity, scene->getFogDensity());
  // Send light infos to the shader
  if (scene->getDirectionalLightsSize())
    {
      _currentProgram->setUniform3(gle::Program::
				    DirectionalLightDirection,
				    scene->getDirectionalLightsDirection(),
				    scene->getDirectionalLightsSize());
      _currentProgram->setUniform3(gle::Program::DirectionalLightColor,
				    scene->getDirectionalLightsColor(),
				    scene->getDirectionalLightsSize());
    }
  if (scene->getPointLightsSize())
    {
      _currentProgram->setUniform3(gle::Program::PointLightPosition,
      			    scene->getPointLightsPosition(),
      			    scene->getPointLightsSize());
      _currentProgram->setUniform3(gle::Program::PointLightColor,
      			    scene->getPointLightsColor(),
      				    scene->getPointLightsSize());
      _currentProgram->setUniform3(gle::Program::PointLightSpecularColor,
      			    scene->getPointLightsSpecularColor(),
      			    scene->getPointLightsSize());
      _currentProgram->setUniform3(gle::Program::PointLightAttenuation,
      			    scene->getPointLightsAttenuation(),
      			    scene->getPointLightsSize());
    }
  if (scene->getSpotLightsSize())
    {
      _currentProgram->setUniform3(gle::Program::SpotLightPosition,
				    scene->getSpotLightsPosition(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3(gle::Program::SpotLightColor,
				    scene->getSpotLightsColor(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3(gle::Program::SpotLightSpecularColor,
				    scene->getSpotLightsSpecularColor(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3(gle::Program::SpotLightAttenuation,
				    scene->getSpotLightsAttenuation(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform3(gle::Program::SpotLightDirection,
				    scene->getSpotLightsDirection(),
				    scene->getSpotLightsSize());
      _currentProgram->setUniform1(gle::Program::SpotLightCosCutOff,
				    scene->getSpotLightsCosCutOff(),
				    scene->getSpotLightsSize());
    }
}
