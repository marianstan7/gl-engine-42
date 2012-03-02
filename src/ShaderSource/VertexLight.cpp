//
// ShaderSource.hpp for  in /home/jochau_g//dev/opengl/glEngine/src
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Tue Feb 21 15:47:17 2012 gael jochaud-du-plessix
// Last update Fri Mar  2 16:20:32 2012 gael jochaud-du-plessix
//

#include <gle/opengl.h>
#include <ShaderSource.hpp>

const char *gle::ShaderSource::Vertex::Light::Head =
  "#define GLE_LIGHT_ENABLED 1\n"
  "#define NB_DIRECTIONAL_LIGHTS %nb_directional_lights \n"
  "#define NB_POINT_LIGHTS %nb_point_lights \n"
  ;

const char *gle::ShaderSource::Vertex::Light::UniformDeclarations =
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
  ;
