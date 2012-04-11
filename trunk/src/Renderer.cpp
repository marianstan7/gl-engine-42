//
// Renderer.cpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Mon Feb 20 20:48:54 2012 gael jochaud-du-plessix
// Last update Wed Apr 11 23:55:59 2012 gael jochaud-du-plessix
//

#include <Renderer.hpp>
#include <gle/opengl.h>
#include <ShaderSource.hpp>

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
  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_BACK);

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

void gle::Renderer::render(Scene* scene)
{
  gle::Color<GLfloat> const & backgroundColor = scene->getBackgroundColor();
  glClearColor(backgroundColor.r, backgroundColor.b, backgroundColor.a,
	       1.f);
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
  GLsizeiptr nbIndexes = mesh->getNbIndexes();
  GLsizeiptr nbVertexes = mesh->getNbVertexes();
  if (nbIndexes < 1 || nbVertexes < 1)
    return ;
  gle::Buffer<GLfloat> * attributesBuffer = mesh->getAttributesBuffer();
  gle::Buffer<GLuint> * indexesBuffer = mesh->getIndexesBuffer();
  gle::Material* material = mesh->getMaterial();

  if (indexesBuffer == NULL || material == NULL)
    return ;

  _setCurrentProgram(material, scene, camera);
  if (!_currentProgram)
    return ;

  _setMeshUniforms(scene, mesh);
  
  attributesBuffer->bind();
  
  // Set Position buffer
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::PositionLocation,
			3, GL_FLOAT, GL_FALSE, 0, 0);

  // Set Normal buffer
  glVertexAttribPointer(gle::ShaderSource::Vertex::Default::NormalLocation,
			3, GL_FLOAT, GL_FALSE, 0,
			(GLvoid*)(nbVertexes
				  * gle::Mesh::VertexAttributeSizeCoords
				  * sizeof(GLfloat)));

  // Set up ColorMap
  if (material->isColorMapEnabled())
    {
      // Set texture coords attribute
      glEnableVertexAttribArray(gle::ShaderSource::Vertex::
                                ColorMap::TextureCoordLocation);
      //textureCoordsBuffer->bind();
      glVertexAttribPointer(gle::ShaderSource::Vertex::
                            ColorMap::TextureCoordLocation,
                            2, GL_FLOAT, GL_FALSE, 0,
			    (GLvoid*)(nbVertexes
				      * (gle::Mesh::VertexAttributeSizeCoords
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
  if (mesh->getType() == gle::Mesh::Points
      || mesh->getRasterizationMode() == gle::Mesh::Point)
    glPointSize(mesh->getPointSize());
  glPolygonMode(GL_FRONT_AND_BACK, mesh->getRasterizationMode());
  glDrawElements(mesh->getType(), nbIndexes, GL_UNSIGNED_INT, 0);

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

  if (!program)
    return ;
  if (program && program != _currentProgram)
    program->use();
  _currentProgram = program;
  _currentMaterial = material;

  _setSceneUniforms(scene, camera);
  _setMaterialUniforms(material);
}

void gle::Renderer::_setMaterialUniforms(gle::Material* material)
{  
  _currentProgram->setUniform(gle::Program::AmbientColor,
			      material->getAmbientColor());
  _currentProgram->setUniform(gle::Program::DiffuseColor,
			      material->getDiffuseColor());
  _currentProgram->setUniform(gle::Program::SpecularColor,
			      material->getSpecularColor());

  _currentProgram->setUniform(gle::Program::Shininess,
			      material->getShininess());
  _currentProgram->setUniform(gle::Program::DiffuseIntensity,
			      material->getDiffuseIntensity());
  _currentProgram->setUniform(gle::Program::SpecularIntensity,
			      material->getSpecularIntensity());
}

void gle::Renderer::_setSceneUniforms(gle::Scene* scene, gle::Camera* camera)
{
  gle::Matrix4<GLfloat> & projectionMatrix = camera->getProjectionMatrix();
  _currentProgram->setUniform(gle::Program::PMatrix, projectionMatrix);

  // Send light infos to the shader
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

void gle::Renderer::_setMeshUniforms(gle::Scene* scene, gle::Mesh* mesh)
{
  gle::Matrix4<GLfloat> mvMatrix = scene->getCurrentCamera()->getModelViewMatrix() * mesh->getMatrix();
  Matrix4<GLfloat> inverse(mvMatrix);
  inverse.inverse();
  gle::Matrix3<GLfloat> normalMatrix = inverse;
  normalMatrix.transpose();

  // Set modelview matrix
  _currentProgram->setUniform(gle::Program::MVMatrix, mvMatrix);

  // Set normals matrix
  _currentProgram->setUniform(gle::Program::NMatrix, normalMatrix);
}
