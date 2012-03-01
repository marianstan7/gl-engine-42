//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Thu Feb 23 16:35:40 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Fragment::Color::Head =
  "#define GLE_COLOR_ENABLED 1\n"
  ;

const char *gle::ShaderSource::Fragment::Color::InputLocations =
  "#define GLE_IN_COLOR_LOCATION 0 \n"
	  ;

const char *gle::ShaderSource::Fragment::Color::InputDeclarations =
  "in vec4 gle_varying_vColor; \n"
  ;

const char *gle::ShaderSource::Fragment::Color::OutputLocations =
  ""
  ;

const char *gle::ShaderSource::Fragment::Color::OutputDeclarations =
  ""
  ;

const char *gle::ShaderSource::Fragment::Color::Body =
  "gle_FragColor = gle_varying_vColor * vec4(gle_varying_vLightWeighting, 1.0); \n"
  ;
