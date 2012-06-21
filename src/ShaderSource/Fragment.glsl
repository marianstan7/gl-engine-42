#version 330 core 
  
#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0 
#define GLE_NB_DIRECTIONAL_LIGHTS %nb_directional_lights 
#define GLE_NB_POINT_LIGHTS %nb_point_lights  
#define GLE_NB_SPOT_LIGHTS %nb_spot_lights 
#define GLE_NB_MATERIALS %nb_materials

#define GLE_CUBE_MAP 1
  
layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor; 

struct gle_MaterialStruct
{
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	float shininess;
	float specularIntensity;
	float diffuseIntensity;
	float reflectionIntensity;
	float envMapType;
	float hasColorMap;
	float hasNormalMap;
};

layout(std140) uniform materialBlock
{
	gle_MaterialStruct materials[GLE_NB_MATERIALS];
} gle_material;

uniform mat4 gle_MVMatrix;

uniform vec3 gle_fogColor;

uniform sampler2D gle_colorMap;
uniform sampler2D gle_normalMap;
uniform samplerCube gle_cubeMap;

#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	uniform vec3 gle_directionalLightColor[GLE_NB_DIRECTIONAL_LIGHTS];
#endif
#if GLE_NB_POINT_LIGHTS > 0
	uniform vec3 gle_pointLightColor[GLE_NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightSpecularColor[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	uniform vec3 gle_spotLightColor[GLE_NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightSpecularColor[GLE_NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightDirection[GLE_NB_SPOT_LIGHTS];
	uniform float gle_spotLightCosCutOff[GLE_NB_SPOT_LIGHTS];
#endif

in float gle_varying_fogFactor; 
in vec3 gle_varying_vLightWeighting;
in float gle_varying_vLightAttenuation;
in vec2 gle_varying_vTextureCoord;
in vec3 gle_varying_envMapVector;

flat in vec3 gle_varying_vMeshIdentifier;
#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
	in vec3 gle_varying_normal;
	in vec3 gle_varying_eyeDirection;
#endif
#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	in vec3 gle_varying_directionalLightDirection[GLE_NB_DIRECTIONAL_LIGHTS];
#endif
#if GLE_NB_POINT_LIGHTS > 0
	in vec3 gle_varying_pointLightDirection[GLE_NB_POINT_LIGHTS];
	in float gle_varying_pointLightAttenuation[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	in vec3 gle_varying_spotLightDirection[GLE_NB_SPOT_LIGHTS];
	in vec3 gle_varying_spotLightRealDirection[GLE_NB_SPOT_LIGHTS];
	in float gle_varying_spotLightAttenuation[GLE_NB_SPOT_LIGHTS];
#endif

void main(void) {

	vec4 ambientColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].ambientColor;
	vec4 diffuseColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].diffuseColor;
	vec4 specularColor = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].specularColor;
	float shininess = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].shininess;
	float specularIntensity = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].specularIntensity;
	float diffuseIntensity = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].diffuseIntensity;
	float reflectionIntensity = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].reflectionIntensity;
	float envMapType = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].envMapType;
	float hasColorMap = gle_material.materials[int(gle_varying_vMeshIdentifier.z)].hasColorMap;
	float hasNormalMap = gle_material.materials[0].hasNormalMap;
	
	vec3 lightWeighting = gle_varying_vLightWeighting;
	vec3 bump = vec3(0.0, 1.0, 0.0);
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
		vec3 N = normalize(gle_varying_normal);
		vec3 E = normalize(gle_varying_eyeDirection);
		if (hasNormalMap > 0.0)
		{
			vec2 tmp = vec2(gle_varying_vTextureCoord.x, -gle_varying_vTextureCoord.y);
			bump = normalize(texture2D(gle_normalMap, tmp).xyz * 2.0 - 1.0);
		}
	#endif
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0
		for (int i = 0; i < GLE_NB_DIRECTIONAL_LIGHTS; ++i)
		{
			if (diffuseIntensity > 0)
			{
				float directionalLightWeighting = 0.0;
				if (hasNormalMap > 0.0)
					directionalLightWeighting = max(dot(bump,
													gle_varying_directionalLightDirection[i]), 0.0);
				else
					directionalLightWeighting = max(dot(N,
													gle_varying_directionalLightDirection[i]), 0.0);
				lightWeighting += gle_directionalLightColor[i] * diffuseColor.rgb * directionalLightWeighting * diffuseIntensity;
			}
			if (specularIntensity > 0)
			{
				float directionalLightWeighting = 0.0;
				if (hasNormalMap > 0.0)
					directionalLightWeighting = pow(max(dot(reflect(-gle_varying_directionalLightDirection[i], bump), E), 0.0), shininess);
				else
					directionalLightWeighting = pow(max(dot(reflect(-gle_varying_directionalLightDirection[i], N), E), 0.0), shininess);
				lightWeighting += gle_directionalLightColor[i] * specularColor.rgb
							* directionalLightWeighting * specularIntensity;
			}
		}
	#endif
	#if GLE_NB_POINT_LIGHTS > 0
		for (int i = 0; i < GLE_NB_POINT_LIGHTS; ++i)
		{
			vec3 L = normalize(gle_varying_pointLightDirection[i]);

			if (diffuseIntensity > 0)
			{
				float pointLightWeighting = 0.0;
				if (hasNormalMap > 0.0)
					pointLightWeighting = max(dot(L, bump), 0.0);
				else
					pointLightWeighting = max(dot(L, N), 0.0);
				lightWeighting += gle_pointLightColor[i] * diffuseColor.rgb * pointLightWeighting * diffuseIntensity * 
				gle_varying_pointLightAttenuation[i];
			}
			if (specularIntensity > 0)
			{
				float pointLightSpecularWeighting = 0.0;
				if (hasNormalMap > 0.0)
					pointLightSpecularWeighting = pow(clamp(dot(reflect(-L, bump), E), 0.0, 1.0), shininess);
				else
					pointLightSpecularWeighting = pow(max(dot(reflect(-L, N), E), 0.0), shininess);
				lightWeighting += gle_pointLightSpecularColor[i] * specularColor.rgb
							* pointLightSpecularWeighting * specularIntensity * gle_varying_pointLightAttenuation[i];
			}
		}
	#endif
	#if GLE_NB_SPOT_LIGHTS > 0
		for (int i = 0; i < GLE_NB_SPOT_LIGHTS; ++i)
		{
			vec3 L = normalize(gle_varying_spotLightDirection[i]);
			vec3 D = normalize(gle_spotLightDirection[i]);

			// COMPUTE SPOT ANGLE
			float cos_cur_angle = dot(-normalize(gle_varying_spotLightRealDirection[i]), D);

			float cos_inner_cone_angle = gle_spotLightCosCutOff[i];
			float cos_outer_cone_angle = cos_inner_cone_angle * 0.95;

			float cos_inner_minus_outer_angle = 
				cos_inner_cone_angle - cos_outer_cone_angle;

			float spot = clamp((cos_cur_angle - cos_outer_cone_angle) / 
					cos_inner_minus_outer_angle, 0.0, 1.0);

			if (diffuseIntensity > 0)
			{
				float spotLightWeighting = 0.0;
				if (hasNormalMap > 0.0)
					spotLightWeighting = max(dot(bump, L), 0.0);
				else
					spotLightWeighting = max(dot(N, L), 0.0);
				lightWeighting += gle_spotLightColor[i] * diffuseColor.rgb * spotLightWeighting * diffuseIntensity * 
				gle_varying_spotLightAttenuation[i] * (spot);
			}
			if (specularIntensity > 0)
			{
				vec3 reflectionDirection = vec3(0.0, 0.0, 0.0);
				if (hasNormalMap > 0.0)
					reflectionDirection = reflect(-L, bump);
				else
					reflectionDirection = reflect(-L, N);
				float spotLightSpecularWeighting = pow(max(dot(reflectionDirection, E), 0.0), shininess);
				lightWeighting += gle_spotLightSpecularColor[i] * specularColor.rgb
							* spotLightSpecularWeighting * specularIntensity * gle_varying_spotLightAttenuation[i] * spot;
			}
		}
	#endif
	vec2 tmp = vec2(gle_varying_vTextureCoord.x, -gle_varying_vTextureCoord.y);
	if (hasColorMap > 0.0)
		gle_FragColor = texture2D(gle_colorMap, tmp) * vec4(lightWeighting, 1.0);
	else
		gle_FragColor = vec4(lightWeighting, 1.0);
	if (reflectionIntensity > 0 && envMapType == GLE_CUBE_MAP)
	{
		vec3 cube_color = texture(gle_cubeMap, gle_varying_envMapVector).rgb;
		gle_FragColor = vec4(mix(vec3(gle_FragColor), cube_color, reflectionIntensity), 1.0);
	}
	gle_FragColor = mix(vec4(gle_fogColor, 1.0), gle_FragColor, gle_varying_fogFactor)	;
}
