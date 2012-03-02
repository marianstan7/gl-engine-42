//
// FragmentColorMap.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu Mar  1 19:53:57 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 18:41:22 2012 loick michard
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Fragment::ColorMap::UniformDeclarations =
  "uniform sampler2D gle_colorMap;\n"
  ;

const char *gle::ShaderSource::Fragment::ColorMap::InputDeclarations =
  "in vec2 gle_varying_vTextureCoord; \n"
  ;

const char *gle::ShaderSource::Fragment::ColorMap::Body =
  "gle_FragColor = texture2D(gle_colorMap, gle_varying_vTextureCoord); \n"
  ;
