//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Thu Feb 23 20:12:47 2012 loick michard
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Vertex::Color::Head =
  "#define GLE_COLOR_ENABLED 1\n"
  ;

const char *gle::ShaderSource::Vertex::Color::UniformDeclarations =
  ""
  ;

GLuint gle::ShaderSource::Vertex::Color::ColorLocation = 2;

const char *gle::ShaderSource::Vertex::Color::InputLocations =
  "#define GLE_IN_VERTEX_COLOR_LOCATION 2 \n"
  ;

const char *gle::ShaderSource::Vertex::Color::InputDeclarations =
  "layout (location = GLE_IN_VERTEX_COLOR_LOCATION) in vec4 gle_vColor; \n"
  ;

const char *gle::ShaderSource::Vertex::Color::OutputLocations =
  "#define GLE_OUT_VERTEX_COLOR_LOCATION 0 \n"
  ;
	
const char *gle::ShaderSource::Vertex::Color::OutputDeclarations =
  "out vec4 gle_varying_vColor; \n"
  ;

const char *gle::ShaderSource::Vertex::Color::Body =
  "gle_varying_vColor = gle_vColor; \n"
  ;
