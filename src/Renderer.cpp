//
// Renderer.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 20:48:54 2012 gael jochaud-du-plessix
// Last update Thu Jun  7 12:45:45 2012 loick michard
//

#include <Renderer.hpp>
#include <gle/opengl.h>
#include <ShaderSource.hpp>
#include <Exception.hpp>

gle::Renderer::Renderer() :
  _currentProgram(NULL), _currentMaterial(NULL)
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

  Camera*			camera = scene->getCurrentCamera();
  gle::Color<GLfloat> const &	backgroundColor = scene->getBackgroundColor();

  glClearColor(backgroundColor.r, backgroundColor.b, backgroundColor.a, 1.f);
  clear();
  _currentProgram = NULL;
    _currentMaterial = NULL;
  if (!camera)
    throw (new gle::Exception::Exception("No camera for the scene..."));
  _setCurrentProgram(scene, camera);
  const std::vector<gle::Mesh*> & meshes = scene->getMeshesToRender();
  auto meshesEnd = meshes.end();
  const std::vector<gle::Mesh*> & unboundingMeshes = scene->getUnboundingMeshesToRender();
  auto unboundingMeshesEnd = unboundingMeshes.end();

  // Enable vertex attributes commons to all meshes
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::PositionLocation);
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::Default::NormalLocation);

  MeshBufferManager::getInstance().bind();

  for (auto it = meshes.begin(); it != meshesEnd; ++it)
    _renderMesh(scene, *it);
  for (auto it = unboundingMeshes.begin(); it != unboundingMeshesEnd; ++it)
    _renderMesh(scene, *it);

  framebuffer.update();
}

void gle::Renderer::_renderMesh(gle::Scene* scene, gle::Mesh* mesh)
{
  GLsizeiptr nbIndexes = mesh->getNbIndexes();
  GLsizeiptr nbVertexes = mesh->getNbVertexes();

  if (nbIndexes < 1 || nbVertexes < 1)
    return ;

  gle::MeshBufferManager::Chunk* vertexAttributes = mesh->getAttributes();
  gle::Buffer<GLuint> * indexesBuffer = mesh->getIndexesBuffer();
  gle::Material* material = mesh->getMaterial();


  if (indexesBuffer == NULL || material == NULL || mesh == NULL ||
      !_currentProgram)
    return ;

  _setMaterialUniforms(material);
  _setMeshUniforms(scene, mesh);
  // Set Position buffer
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::PositionLocation,
			3, GL_FLOAT, GL_FALSE,
			gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)(vertexAttributes->getOffset() * sizeof(GLfloat)));

  // Set Normal buffer
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::NormalLocation,
			3, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			(GLvoid*)((vertexAttributes->getOffset()
				   + gle::Mesh::VertexAttributeSizeCoords)
				  * sizeof(GLfloat)));

  // Set up ColorMap
  _currentProgram->setUniform(gle::Program::HasColorMap, material->isColorMapEnabled());
  if (material->isColorMapEnabled())
    {
      // Set texture coords attribute
      glEnableVertexAttribArray(gle::ShaderSource::Vertex::
                                ColorMap::TextureCoordLocation);
      //textureCoordsBuffer->bind();
      glVertexAttribPointer(gle::ShaderSource::Vertex::
                            ColorMap::TextureCoordLocation,
                            2, GL_FLOAT, GL_FALSE, gle::Mesh::VertexAttributesSize * sizeof(GLfloat),
			    (GLvoid*)((vertexAttributes->getOffset()
				       + gle::Mesh::VertexAttributeSizeCoords
				       + gle::Mesh::VertexAttributeSizeNormal)
				      * sizeof(GLfloat)));

      // Set texture to the shader
      gle::Texture* colorMap = material->getColorMap();
      glActiveTexture(gle::Program::ColorMapTexture);
      colorMap->bind();
      _currentProgram->setUniform(gle::Program::ColorMap,
      				  gle::Program::ColorMapTextureIndex);
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
  glDrawElements(mesh->getPrimitiveType(), nbIndexes, GL_UNSIGNED_INT, 0);

  if (material->isColorMapEnabled())
    glDisableVertexAttribArray(gle::ShaderSource::Vertex::
			       ColorMap::TextureCoordLocation);
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

void gle::Renderer::_setMaterialUniforms(gle::Material* material)
{  
  material->getUniforms()->bindBase(gle::Program::MaterialBlock);
}

void gle::Renderer::_setSceneUniforms(gle::Scene* scene, gle::Camera* camera)
{
  const gle::Matrix4<GLfloat> & projectionMatrix = camera->getProjectionMatrix();

  _currentProgram->setUniform(gle::Program::PMatrix, projectionMatrix);
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

void gle::Renderer::_setMeshUniforms(gle::Scene* scene, gle::Mesh* mesh)
{
  const gle::Matrix4<GLfloat>& mvMatrix = scene->getCurrentCamera()->getTransformationMatrix() * mesh->getTransformationMatrix();

  gle::Matrix4<GLfloat> inverse(mvMatrix);
  inverse.inverse();
  gle::Matrix3<GLfloat> normalMatrix = inverse;
  normalMatrix.transpose();
  // Set modelview matrix
  _currentProgram->setUniform(gle::Program::MVMatrix, mvMatrix);
  // Set normals matrix
  if (scene->getDirectionalLightsSize() || scene->getPointLightsSize() || scene->getSpotLightsSize())
    _currentProgram->setUniform(gle::Program::NMatrix, normalMatrix);
  /*
  glUniformBlockBinding(_currentProgram->getId(), 
			_currentProgram->getUniformBlockIndex(gle::Program::MaterialBlock),
			gle::Program::MaterialBlock);
			uniforms->bindBase(gle::Program::MaterialBlock);
  */
}
