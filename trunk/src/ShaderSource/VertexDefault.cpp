//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Thu Jun 21 00:14:15 2012 loick michard
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Vertex::BodyBegin =
  "\n"
  "void main(void) { \n"
  "\n"
  ;

const char *gle::ShaderSource::Vertex::BodyEnd =
  "\n"
  "} \n"
  ;

const char *gle::ShaderSource::Vertex::Default::Head =
  "#version 330 core \n"
  ;

const char *gle::ShaderSource::Vertex::Default::UniformDeclarations =
  "uniform mat4 gle_PMatrix; \n"
  "uniform mat4 gle_MVMatrix; \n"
  "uniform mat3 gle_NMatrix;\n"
  ;
	
GLuint gle::ShaderSource::Vertex::Default::PositionLocation = 0;
GLuint gle::ShaderSource::Vertex::Default::NormalLocation = 1;
GLuint gle::ShaderSource::Vertex::Default::TangentLocation = 2;
GLuint gle::ShaderSource::Vertex::Default::MeshIdentifierLocation = 4;

const char * gle::ShaderSource::Vertex::Default::InputLocations =
  "#define GLE_IN_VERTEX_POSITION_LOCATION 0 \n"
  "#define GLE_IN_VERTEX_NORMAL_LOCATION 1 \n"
  ;

const char *gle::ShaderSource::Vertex::Default::InputDeclarations =
  "layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition; \n"
  "layout (location = GLE_IN_VERTEX_NORMAL_LOCATION) in vec3 gle_vNormal; \n"
  ;
	
const char *gle::ShaderSource::Vertex::Default::OutputLocations =
  ""
  ;
	
const char *gle::ShaderSource::Vertex::Default::OutputDeclarations =
  "out vec3 gle_varying_vPosition; \n"
  "out vec3 gle_varying_vLightWeighting; \n"
  ;

const char *gle::ShaderSource::Vertex::Default::Body =
  "vec4 gle_mvPosition = gle_MVMatrix * vec4(gle_vPosition, 1.0); \n"
  "gl_Position = gle_PMatrix * gle_mvPosition; \n"
  "gle_varying_vPosition = gle_vPosition; \n"
  "vec3 transformedNormal = normalize(gle_NMatrix * gle_vNormal);\n"
  "gle_varying_vLightWeighting = vec3(1.0, 1.0, 1.0);\n"
  ;
