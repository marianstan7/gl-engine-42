//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Wed Feb 29 12:07:55 2012 loick michard
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Vertex::Light::Head =
  "#define GLE_LIGHT_ENABLED 1\n"
  "#define NB_DIRECTIONAL_LIGHTS %nb_directional_lights \n"
  "#define NB_POINT_LIGHTS %nb_point_lights \n"
  ;

const char *gle::ShaderSource::Vertex::Light::UniformDeclarations =
  "uniform vec3 ambientColor;\n"
  "uniform float shininess;\n"
  "uniform float specularIntensity;\n"
  "#if NB_DIRECTIONAL_LIGHTS > 0\n"
  "uniform vec3 directionalLightDirection[NB_DIRECTIONAL_LIGHTS];\n"
  "uniform vec3 directionalLightColor[NB_DIRECTIONAL_LIGHTS];\n"
  "#endif\n"
  "#if NB_POINT_LIGHTS > 0\n"
  "uniform vec3 pointLightPosition[NB_POINT_LIGHTS];\n"
  "uniform vec3 pointLightColor[NB_POINT_LIGHTS];\n"
  "uniform vec3 pointLightSpecularColor[NB_POINT_LIGHTS];\n"
  "#endif\n"
  ;

const char *gle::ShaderSource::Vertex::Light::InputLocations =
  ""
  ;

const char *gle::ShaderSource::Vertex::Light::InputDeclarations =
  ""
  ;

const char *gle::ShaderSource::Vertex::Light::OutputLocations =
  ""
  ;
	
const char *gle::ShaderSource::Vertex::Light::OutputDeclarations =
  ""
  ;

const char *gle::ShaderSource::Vertex::Light::Body =
  "gle_varying_vLightWeighting = vec3(0.0, 0.0, 0.0);\n"
  "#if NB_DIRECTIONAL_LIGHTS > 0\n"
  "for (int i = 0; i < NB_DIRECTIONAL_LIGHTS; ++i) {\n"
  "float directionalLightWeighting = max(dot(transformedNormal, directionalLightDirection[i]), 0.0);\n"
  "gle_varying_vLightWeighting += directionalLightColor[i] * directionalLightWeighting;\n"
  "}\n"
  "#endif\n"
  "#if NB_POINT_LIGHTS > 0\n"
  "for (int i = 0; i < NB_POINT_LIGHTS; ++i) {\n"
  "vec3 pointLightDirection = normalize(pointLightPosition[i] - gle_mvPosition.xyz);\n"
  "float pointLightWeighting = max(dot(transformedNormal, pointLightDirection), 0.0);\n"
  "gle_varying_vLightWeighting += pointLightColor[i] * pointLightWeighting;\n"
  "if (specularIntensity > 0) {\n"
  "vec3 eyeDirection = normalize(-gle_mvPosition.xyz);\n"
  "vec3 reflectionDirection = reflect(-pointLightDirection, transformedNormal);\n"
  "float pointLightSpecularWeighting = pow(max(dot(-reflectionDirection, eyeDirection), 0.0), shininess);\n"
  "gle_varying_vLightWeighting += pointLightSpecularColor[i] * pointLightSpecularWeighting * specularIntensity;\n"
  "} }\n"
  "#endif\n"
  "gle_varying_vLightWeighting += ambientColor;\n"
  ;
