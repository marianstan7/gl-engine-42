//
// FragmentLight.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu Mar  1 18:00:40 2012 gael jochaud-du-plessix
// Last update Fri Mar 16 11:32:38 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Fragment::Light::InputDeclarations =
  "in vec3 gle_varying_vLightWeighting; \n"
  ;

const char *gle::ShaderSource::Fragment::Light::Body =
  "gle_FragColor *= vec4(gle_varying_vLightWeighting, 1);\n"
  //"gle_FragColor.a = gle_FragColor.b;"
  ;
