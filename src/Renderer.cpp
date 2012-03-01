//
// Renderer.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 20:48:54 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 21:29:21 2012 gael jochaud-du-plessix
//

#include <Renderer.hpp>
#include <gle/opengl.h>
#include <ShaderSource.hpp>

gle::Renderer::Renderer() :
  _currentProgram(NULL), _currentMaterial(NULL)
{
  // Set color and depth clear value
  glClearDepth(1.f);
  glClearColor(0.f, 0.f, 0.f, 1.f);

  // Enable Z-buffer read and write 
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

  // Enable antialiasing
  // glEnable(GL_LINE_SMOOTH);
  // glEnable(GL_POLYGON_SMOOTH);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

gle::Renderer::~Renderer()
{

}

void gle::Renderer::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gle::Renderer::render(Scene* scene)
{
  clear();
  _currentProgram = NULL;
  _currentMaterial = NULL;
  Camera* camera = scene->getCurrentCamera();
  std::vector<gle::Mesh*> & meshes = scene->getMeshes();
  std::vector<gle::Mesh*>::iterator meshesEnd = meshes.end();

  // Enable vertex attributes commons to all meshes
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::
			    Default::PositionLocation);
  glEnableVertexAttribArray(gle::ShaderSource::Vertex::
			    Default::NormalLocation);

  for (std::vector<gle::Mesh*>::iterator it = meshes.begin();
       it != meshesEnd; ++it)
    _renderMesh(scene, *it, camera);
}

void gle::Renderer::createPrograms(gle::Scene* scene)
{
  std::vector<gle::Material*> & materials = scene->getMaterials();
  std::map<gle::Material*, gle::Program*> & programs = scene->getPrograms();

  // First, clear the old programs list
  for (std::map<gle::Material*, gle::Program*>::iterator it = programs.begin(),
	 end = programs.end();
       it != end; ++it)
    delete it->second;
  programs.clear();
  // Then create the OpenGL Programs for each material of the scene
  for (std::vector<gle::Material*>::iterator it = materials.begin(),
	 end = materials.end();
       it != end; ++it)
    programs[*it] = (*it)->createProgram(scene);
}

void gle::Renderer::_renderMesh(gle::Scene* scene, gle::Mesh* mesh,
				gle::Camera* camera)
{
  gle::Buffer<GLfloat> * vertexesBuffer = mesh->getVertexesBuffer();
  gle::Buffer<GLfloat> * normalsBuffer = mesh->getNormalsBuffer();
  gle::Buffer<GLuint> * indexesBuffer = mesh->getIndexesBuffer();
  gle::Material* material = mesh->getMaterial();

  if (vertexesBuffer == NULL || normalsBuffer == NULL || indexesBuffer == NULL
      || material == NULL)
    return ;

  _setCurrentProgram(material, scene, camera);

  _setMeshUniforms(material, scene, mesh);
  
  // Set Position buffer
  vertexesBuffer->bind();
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::PositionLocation,
			3, GL_FLOAT, GL_FALSE, 0, 0);

  // Set Normal buffer
  normalsBuffer->bind();
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::NormalLocation,
			3, GL_FLOAT, GL_FALSE, 0, 0);

  // Set up ColorMap
  if (material->isColorMapEnabled())
    {
      // Set texture coords attribute
      gle::Buffer<GLfloat> *textureCoordsBuffer =
        mesh->getTextureCoordsBuffer();
      glEnableVertexAttribArray(gle::ShaderSource::Vertex::
                                ColorMap::TextureCoordLocation);
      textureCoordsBuffer->bind();
      glVertexAttribPointer(gle::ShaderSource::Vertex::
                            ColorMap::TextureCoordLocation,
                            2, GL_FLOAT, GL_FALSE, 0, 0);
      // Set texture to the shader
      gle::Texture* colorMap = material->getColorMap();
      glActiveTexture(gle::Program::ColorMapTexture);
      colorMap->bind();
      _currentProgram->setUniform(gle::Program::ColorMap,
      				  gle::Program::ColorMapTextureIndex);
    }

  // Draw the mesh elements
  indexesBuffer->bind();
  glDrawElements(GL_TRIANGLES, mesh->getNbIndexes(), GL_UNSIGNED_INT, 0);

  if (material->isColorMapEnabled())
    glDisableVertexAttribArray(gle::ShaderSource::Vertex::
			       ColorMap::TextureCoordLocation);
}

void gle::Renderer::_setCurrentProgram(gle::Material* material,
				       gle::Scene* scene,
				       gle::Camera* camera)
{
  if (material == _currentMaterial)
    return ;
  std::map<gle::Material*, gle::Program*> & programs = scene->getPrograms();
  gle::Program* program = programs[material];

  if (program && program != _currentProgram)
    program->use();
  _currentProgram = program;
  _currentMaterial = material;

  _setSceneUniforms(material, scene, camera);
  _setMaterialUniforms(material, scene);
}

void gle::Renderer::_setMaterialUniforms(gle::Material* material,
					 gle::Scene* scene)
{
  _currentProgram->setUniform(gle::Program::Color, material->getColor());
  if (material->isLightEnabled() && scene->isLightEnabled())
    {
      _currentProgram->setUniform(gle::Program::Shininess,
				    material->getShininess());
      _currentProgram->setUniform(gle::Program::DiffuseIntensity,
				    material->getDiffuseIntensity());
      _currentProgram->setUniform(gle::Program::SpecularIntensity,
				    material->getSpecularIntensity());
    }
}

void gle::Renderer::_setSceneUniforms(gle::Material* material,
				      gle::Scene* scene, gle::Camera* camera)
{
  gle::Matrix4<GLfloat> & projectionMatrix = camera->getProjectionMatrix();
  _currentProgram->setUniform(gle::Program::PMatrix, projectionMatrix);

  // Send light infos to the shader
  if (material->isLightEnabled() && scene->isLightEnabled())
    {
      _currentProgram->setUniform3v(gle::Program::AmbientColor,
				    scene->getAmbientColor(),
				    1);
      if (scene->getDirectionalLightsSize())
	{
	  _currentProgram->setUniform3v(gle::Program::
					DirectionalLightDirection,
					scene->getDirectionalLightsDirection(),
					scene->getDirectionalLightsSize());
	  _currentProgram->setUniform3v(gle::Program::DirectionalLightColor,
					scene->getDirectionalLightsColor(),
					scene->getDirectionalLightsSize());
	}
      if (scene->getPointLightsSize())
	{
	  _currentProgram->setUniform3v(gle::Program::PointLightPosition,
					scene->getPointLightsPosition(),
					scene->getPointLightsSize());
	  _currentProgram->setUniform3v(gle::Program::PointLightColor,
					scene->getPointLightsColor(),
					scene->getPointLightsSize());
	  _currentProgram->setUniform3v(gle::Program::PointLightSpecularColor,
					scene->getPointLightsSpecularColor(),
					scene->getPointLightsSize());
	}
    }
  
}

void gle::Renderer::_setMeshUniforms(gle::Material* material,
				     gle::Scene* scene, gle::Mesh* mesh)
{
  gle::Matrix4<GLfloat> mvMatrix = scene->getCurrentCamera()->getModelViewMatrix() * mesh->getMatrix();
  Matrix4<GLfloat> inverse(mvMatrix);
  inverse.inverse();
  gle::Matrix3<GLfloat> normalMatrix = inverse;
  normalMatrix.transpose();

  // Set modelview matrix
  _currentProgram->setUniform(gle::Program::MVMatrix, mvMatrix);

  // Set normals matrix
  if (material->isLightEnabled() && scene->isLightEnabled() &&
      scene->hasLights())
    _currentProgram->setUniform(gle::Program::NMatrix, normalMatrix);
}
