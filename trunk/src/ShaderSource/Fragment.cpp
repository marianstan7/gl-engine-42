//
// Fragment.cpp for glEngine in /home/michar_l//gl-engine-42/src/ShaderSource
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed Apr 11 23:15:58 2012 loick michard
// Last update Tue May 29 23:16:27 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::FragmentShader =
  "#version 330 core \n"
  
  "#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0 \n"
  
  "layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor; \n"

  "uniform vec3 gle_fogColor;\n"

  "uniform sampler2D gle_colorMap;\n"
  "uniform bool gle_hasColorMap;\n"

  "in float gle_varying_fogFactor; \n"
  "in vec3 gle_varying_vLightWeighting; \n"
  "in vec2 gle_varying_vTextureCoord; \n"

  "void main(void) { \n"
  "vec2 tmp = vec2(gle_varying_vTextureCoord.x, -gle_varying_vTextureCoord.y);\n"
  "if (gle_hasColorMap){\n"
  "gle_FragColor = texture2D(gle_colorMap, tmp);\n"
  "} else {\n"
  "gle_FragColor = vec4(1.0);\n"
  "}\n"
  "gle_FragColor *= vec4(gle_varying_vLightWeighting, 1.0);\n"
  "gle_FragColor = mix(vec4(gle_fogColor, 1.0), gle_FragColor, gle_varying_fogFactor);\n"
  "} \n"
  ;
