//
// Vertex.cpp for glEngine in /home/michar_l//gl-engine-42/src/ShaderSource
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Wed Apr 11 18:41:59 2012 loick michard
// Last update Thu Apr 12 00:02:58 2012 loick michard
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::VertexShader =
  "#version 330 core \n"

  "#define GLE_IN_VERTEX_POSITION_LOCATION 0 \n"
  "#define GLE_IN_VERTEX_NORMAL_LOCATION 1 \n"

  "#define GLE_IN_VERTEX_TEXTURE_COORD_LOCATION 2 \n"

  "#define GLE_LIGHT_ENABLED 1\n"
  "#define NB_DIRECTIONAL_LIGHTS %nb_directional_lights \n"
  "#define NB_POINT_LIGHTS %nb_point_lights \n"

  "uniform mat4 gle_PMatrix; \n"
  "uniform mat4 gle_MVMatrix; \n"
  "uniform mat3 gle_NMatrix;\n"

  "uniform vec3 gle_ambientColor;\n"
  "uniform vec3 gle_diffuseColor;\n"
  "uniform vec3 gle_specularColor;\n"
  "uniform float gle_shininess;\n"
  "uniform float gle_specularIntensity;\n"
  "uniform float gle_diffuseIntensity;\n"
  "#if NB_DIRECTIONAL_LIGHTS > 0\n"
  "uniform vec3 gle_directionalLightDirection[NB_DIRECTIONAL_LIGHTS];\n"
  "uniform vec3 gle_directionalLightColor[NB_DIRECTIONAL_LIGHTS];\n"
  "#endif\n"
  "#if NB_POINT_LIGHTS > 0\n"
  "uniform vec3 gle_pointLightPosition[NB_POINT_LIGHTS];\n"
  "uniform vec3 gle_pointLightColor[NB_POINT_LIGHTS];\n"
  "uniform vec3 gle_pointLightSpecularColor[NB_POINT_LIGHTS];\n"
  "#endif\n"

  "layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition; \n"
  "layout (location = GLE_IN_VERTEX_NORMAL_LOCATION) in vec3 gle_vNormal; \n"
  "layout (location = GLE_IN_VERTEX_TEXTURE_COORD_LOCATION) in vec2 gle_vTextureCoord; \n" 
 
  "out vec3 gle_varying_vPosition; \n"
  "out vec3 gle_varying_vLightWeighting; \n"
  "out vec2 gle_varying_vTextureCoord; \n"

  "\n"
  "void main(void) { \n"

  "vec4 gle_mvPosition = gle_MVMatrix * vec4(gle_vPosition, 1.0); \n"
  "gl_Position = gle_PMatrix * gle_mvPosition; \n"
  "gle_varying_vPosition = gle_vPosition; \n"
  "vec3 transformedNormal = normalize(gle_NMatrix * gle_vNormal);\n"
  "gle_varying_vLightWeighting = vec3(1.0, 1.0, 1.0);\n"

  "gle_varying_vTextureCoord = gle_vTextureCoord; \n"

  "gle_varying_vLightWeighting = vec3(0.0, 0.0, 0.0);\n"
  "#if NB_DIRECTIONAL_LIGHTS > 0\n"
  "for (int i = 0; i < NB_DIRECTIONAL_LIGHTS; ++i) {\n"
  "float directionalLightWeighting = max(dot(transformedNormal, gle_directionalLightDirection[i]), 0.0);\n"
  "gle_varying_vLightWeighting += gle_directionalLightColor[i] * gle_diffuseColor * directionalLightWeighting;\n"
  "}\n"
  "#endif\n"
  "#if NB_POINT_LIGHTS > 0\n"
  "for (int i = 0; i < NB_POINT_LIGHTS; ++i) {\n"
  "vec3 pointLightDirection = normalize(gle_pointLightPosition[i] - gle_mvPosition.xyz);\n"
  "if (gle_diffuseIntensity > 0) {\n"
  "float pointLightWeighting = max(dot(transformedNormal, pointLightDirection), 0.0);\n"
  "gle_varying_vLightWeighting += gle_pointLightColor[i] * gle_diffuseColor * pointLightWeighting * gle_diffuseIntensity;\n"
  "} \n"
  "if (gle_specularIntensity > 0) {\n"
  "vec3 eyeDirection = normalize(-gle_mvPosition.xyz);\n"
  "vec3 reflectionDirection = reflect(-pointLightDirection, transformedNormal);\n"
  "float pointLightSpecularWeighting = pow(max(dot(reflectionDirection, eyeDirection), 0.0), gle_shininess);\n"
  "gle_varying_vLightWeighting += gle_pointLightSpecularColor[i] * gle_specularColor * pointLightSpecularWeighting * gle_specularIntensity;\n"
  "} }\n"
  "#endif\n"
  "gle_varying_vLightWeighting += gle_ambientColor;\n"
  "} \n"
  ;
