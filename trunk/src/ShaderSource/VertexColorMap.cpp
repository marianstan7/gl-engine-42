//
// VertexColorMap.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu Mar  1 19:41:10 2012 gael jochaud-du-plessix
// Last update Thu Mar  1 21:02:47 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

GLuint gle::ShaderSource::Vertex::ColorMap::TextureCoordLocation = 2;

const char * gle::ShaderSource::Vertex::ColorMap::InputLocations =
  "#define GLE_IN_VERTEX_TEXTURE_COORD_LOCATION 2 \n"
  ;

const char *gle::ShaderSource::Vertex::ColorMap::InputDeclarations =
  "layout (location = GLE_IN_VERTEX_TEXTURE_COORD_LOCATION) in vec2 gle_vTextureCoord; \n"
  ;

const char *gle::ShaderSource::Vertex::ColorMap::OutputDeclarations =
  "out vec2 gle_varying_vTextureCoord; \n"
  ;

const char *gle::ShaderSource::Vertex::ColorMap::Body =
  "gle_varying_vTextureCoord = gle_vTextureCoord; \n"
  ;
