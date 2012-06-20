#version 330 core 
  
#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0 
#define NB_POINT_LIGHTS %nb_point_lights  
#define NB_SPOT_LIGHTS %nb_spot_lights

#define GLE_NB_MATERIALS %nb_materials

layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor;

struct gle_MaterialStruct
{
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
	float specularIntensity;
	float diffuseIntensity;
};

layout(std140) uniform materialBlock
{
/*
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
	float specularIntensity;
	float diffuseIntensity;
*/
	gle_MaterialStruct materials[GLE_NB_MATERIALS];
} gle_material;

uniform mat4 gle_MVMatrix; 

uniform vec3 gle_fogColor;

uniform sampler2D gle_colorMap;
uniform bool gle_hasColorMap;

#if NB_POINT_LIGHTS > 0
	uniform vec3 gle_pointLightColor[NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightSpecularColor[NB_POINT_LIGHTS];
#endif
#if NB_SPOT_LIGHTS > 0
	uniform vec3 gle_spotLightColor[NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightSpecularColor[NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightDirection[NB_SPOT_LIGHTS];
	uniform float gle_spotLightCosCutOff[NB_SPOT_LIGHTS];
#endif

in float gle_varying_fogFactor; 
in vec3 gle_varying_vLightWeighting;
in float gle_varying_vLightAttenuation;
in vec2 gle_varying_vTextureCoord;
flat in vec3 gle_varying_vMeshIdentifier;

#if NB_POINT_LIGHTS > 0 || NB_SPOT_LIGHTS > 0
	in vec3 gle_varying_normal;
	in vec3 gle_varying_eyeDirection;
#endif
#if NB_POINT_LIGHTS > 0
	in vec3 gle_varying_pointLightDirection[NB_POINT_LIGHTS];
	in float gle_varying_pointLightAttenuation[NB_POINT_LIGHTS];
#endif
#if NB_SPOT_LIGHTS > 0
	in vec3 gle_varying_spotLightDirection[NB_SPOT_LIGHTS];
	in float gle_varying_spotLightAttenuation[NB_SPOT_LIGHTS];
#endif

void main(void) {

	vec4 ambientColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].ambientColor;
	vec4 diffuseColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].diffuseColor;
	vec4 specularColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].specularColor;
	float shininess = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].shininess;
	float specularIntensity = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].specularIntensity;
	float diffuseIntensity = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].diffuseIntensity;
	
	vec3 lightWeighting = gle_varying_vLightWeighting;
	#if NB_POINT_LIGHTS > 0 || NB_SPOT_LIGHTS > 0
		vec3 N = normalize(gle_varying_normal);
		vec3 E = normalize(gle_varying_eyeDirection);
	#endif
	#if NB_POINT_LIGHTS > 0
		for (int i = 0; i < NB_POINT_LIGHTS; ++i)
		{
			vec3 L = normalize(gle_varying_pointLightDirection[i]);

			if (diffuseIntensity > 0)
			{
				float pointLightWeighting = max(dot(N, L), 0.0);
				lightWeighting += gle_pointLightColor[i] * diffuseColor.rgb * pointLightWeighting * diffuseIntensity * 
				gle_varying_pointLightAttenuation[i];
			}
			if (specularIntensity > 0)
			{
				vec3 reflectionDirection = reflect(-L, N);
				float pointLightSpecularWeighting = pow(max(dot(reflectionDirection, E), 0.0), shininess);
				lightWeighting += gle_pointLightSpecularColor[i] * specularColor.rgb
							* pointLightSpecularWeighting * specularIntensity * gle_varying_pointLightAttenuation[i];
			}
		}
	#endif
	#if NB_SPOT_LIGHTS > 0
		for (int i = 0; i < NB_SPOT_LIGHTS; ++i)
		{
			vec3 L = normalize(gle_varying_spotLightDirection[i]);
			vec3 D = normalize(gle_spotLightDirection[i]);

			// COMPUTE SPOT ANGLE
			float cos_cur_angle = dot(-L, D);

			float cos_inner_cone_angle = gle_spotLightCosCutOff[i];
			float cos_outer_cone_angle = cos_inner_cone_angle * 0.95;

			float cos_inner_minus_outer_angle = 
				cos_inner_cone_angle - cos_outer_cone_angle;

			float spot = clamp((cos_cur_angle - cos_outer_cone_angle) / 
					cos_inner_minus_outer_angle, 0.0, 1.0);

			if (diffuseIntensity > 0)
			{
				float spotLightWeighting = max(dot(N, L), 0.0);
				lightWeighting += gle_spotLightColor[i] * diffuseColor.rgb * spotLightWeighting * diffuseIntensity * 
				gle_varying_spotLightAttenuation[i] * (spot);
			}
			if (specularIntensity > 0)
			{
				vec3 reflectionDirection = reflect(-L, N);
				float spotLightSpecularWeighting = pow(max(dot(reflectionDirection, E), 0.0), shininess);
				lightWeighting += gle_spotLightSpecularColor[i] * specularColor.rgb
							* spotLightSpecularWeighting * specularIntensity * gle_varying_spotLightAttenuation[i] * spot;
			}
		}
	#endif

	vec2 tmp = vec2(gle_varying_vTextureCoord.x, -gle_varying_vTextureCoord.y);
	if (gle_hasColorMap)
	{
		gle_FragColor = texture2D(gle_colorMap, tmp) * vec4(lightWeighting, 1.0);
	}
	else
	{
		gle_FragColor = vec4(lightWeighting, 1.0);
	}
	gle_FragColor = mix(vec4(gle_fogColor, 1.0), gle_FragColor, gle_varying_fogFactor);
}
