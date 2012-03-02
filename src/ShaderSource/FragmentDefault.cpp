//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 16:26:17 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>
      
const char *gle::ShaderSource::Fragment::BodyBegin =
  "\n"
  "void main(void) { \n"
  "\n"
  ;

const char *gle::ShaderSource::Fragment::BodyEnd =
  "\n"
  "} \n"
  ;

const char *gle::ShaderSource::Fragment::Default::Head =
  "#version 330 core \n"
  ;

const char *gle::ShaderSource::Fragment::Default::UniformDeclarations =
  ""
  ;

const char * gle::ShaderSource::Fragment::Default::InputLocations =
  ""
	  ;

const char *gle::ShaderSource::Fragment::Default::InputDeclarations =
  ""
  ;

const char *gle::ShaderSource::Fragment::Default::OutputLocations =
  "#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0 \n"
	  ;

const char *gle::ShaderSource::Fragment::Default::OutputDeclarations =
  "layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor; \n"
  ;

const char *gle::ShaderSource::Fragment::Default::Body =
  "gle_FragColor = vec4(1.0);\n"
  ;
