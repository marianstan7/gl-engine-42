#version 330 core 

#define GLE_IN_VERTEX_POSITION_LOCATION 0 
#define GLE_IN_VERTEX_NORMAL_LOCATION 1 

#define GLE_IN_VERTEX_TEXTURE_COORD_LOCATION 2 

#define GLE_LIGHT_ENABLED 1
#define NB_DIRECTIONAL_LIGHTS %nb_directional_lights 
#define NB_POINT_LIGHTS %nb_point_lights 

uniform mat4 gle_PMatrix; 
uniform float gle_fogDensity;
#if NB_DIRECTIONAL_LIGHTS > 0
	uniform vec3 gle_directionalLightDirection[NB_DIRECTIONAL_LIGHTS];
	uniform vec3 gle_directionalLightColor[NB_DIRECTIONAL_LIGHTS];
#endif
#if NB_POINT_LIGHTS > 0
	uniform vec3 gle_pointLightPosition[NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightColor[NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightSpecularColor[NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightAttenuation[NB_POINT_LIGHTS];
#endif

uniform mat4 gle_MVMatrix; 
#if NB_DIRECTIONAL_LIGHTS > 0 || NB_POINT_LIGHTS > 0
	uniform mat3 gle_NMatrix;
#endif

layout(std140) uniform materialBlock
{
	uniform vec4 gle_ambientColor;
	uniform vec4 gle_diffuseColor;
	uniform vec4 gle_specularColor;
	uniform float gle_shininess;
	uniform float gle_specularIntensity;
	uniform float gle_diffuseIntensity;
};

layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition; 
layout (location = GLE_IN_VERTEX_NORMAL_LOCATION) in vec3 gle_vNormal; 
layout (location = GLE_IN_VERTEX_TEXTURE_COORD_LOCATION) in vec2 gle_vTextureCoord;

out float gle_varying_fogFactor; 
out vec3 gle_varying_vLightWeighting; 
out float gle_varying_vLightAttenuation;
out vec2 gle_varying_vTextureCoord;

void main(void) { 
     	gle_varying_vLightAttenuation = 0.0;
	vec4 gle_mvPosition = gle_MVMatrix * vec4(gle_vPosition, 1.0);
	gl_Position = gle_PMatrix * gle_mvPosition; 
	float fogDistance = length(gle_mvPosition); 
	const float LOG2 = 1.442695; 
	gle_varying_fogFactor = exp2(-gle_fogDensity * gle_fogDensity * fogDistance * fogDistance * LOG2); 
	gle_varying_fogFactor = clamp(gle_varying_fogFactor, 0.0, 1.0); 
	#if NB_DIRECTIONAL_LIGHTS > 0 || NB_POINT_LIGHTS > 0
		vec3 transformedNormal = normalize(gle_NMatrix * gle_vNormal);
	#endif
	gle_varying_vLightWeighting = vec3(1.0, 1.0, 1.0);

	gle_varying_vTextureCoord = gle_vTextureCoord; 

	gle_varying_vLightWeighting = vec3(0.0, 0.0, 0.0);

	#if NB_DIRECTIONAL_LIGHTS > 0
		for (int i = 0; i < NB_DIRECTIONAL_LIGHTS; ++i)
		{
			float directionalLightWeighting = max(dot(transformedNormal, gle_directionalLightDirection[i]), 0.0);
			gle_varying_vLightWeighting += gle_directionalLightColor[i] * gle_diffuseColor.rgb * directionalLightWeighting;
			gle_varying_vLightAttenuation += 1.0
		}
	#endif
	#if NB_POINT_LIGHTS > 0
		for (int i = 0; i < NB_POINT_LIGHTS; ++i)
		{
			vec3 pointLightDirection = gle_pointLightPosition[i] - gle_mvPosition.xyz;
			float d = length(pointLightDirection);
			if (gle_pointLightAttenuation[i].x == 0.0 && gle_pointLightAttenuation[i].y == 0.0 && gle_pointLightAttenuation[i].z == 0.0)
			   gle_varying_vLightAttenuation += 1.0;
			else
			   gle_varying_vLightAttenuation += 1.0 / (gle_pointLightAttenuation[i].x +
			   			      	 (gle_pointLightAttenuation[i].y * d) + 
							 (gle_pointLightAttenuation[i].z * d * d));
2			pointLightDirection = normalize(pointLightDirection);
			if (gle_diffuseIntensity > 0)
			{
				float pointLightWeighting = max(dot(transformedNormal, pointLightDirection), 0.0);
				gle_varying_vLightWeighting += gle_pointLightColor[i] * gle_diffuseColor.rgb * pointLightWeighting * gle_diffuseIntensity;
			}
			if (gle_specularIntensity > 0)
			{
				vec3 eyeDirection = normalize(-gle_mvPosition.xyz);
				vec3 reflectionDirection = reflect(-pointLightDirection, transformedNormal);
				float pointLightSpecularWeighting = pow(max(dot(reflectionDirection, eyeDirection), 0.0), gle_shininess);
				gle_varying_vLightWeighting += gle_pointLightSpecularColor[i] * gle_specularColor.rgb
							* pointLightSpecularWeighting * gle_specularIntensity;
			}
		}
	#endif
	
	gle_varying_vLightWeighting += gle_ambientColor.rgb;
}
