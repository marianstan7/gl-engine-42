#version 330 core 

#define GLE_IN_VERTEX_POSITION_LOCATION 0
#define GLE_IN_VERTEX_NORMAL_LOCATION 1
#define GLE_IN_VERTEX_TEXTURE_COORD_LOCATION 2
#define GLE_IN_VERTEX_MESH_ID_LOCATION 3

#define GLE_LIGHT_ENABLED 1
#define GLE_NB_DIRECTIONAL_LIGHTS %nb_directional_lights 
#define GLE_NB_POINT_LIGHTS %nb_point_lights
#define GLE_NB_SPOT_LIGHTS %nb_spot_lights

#define GLE_NB_STATIC_MESHES %nb_static_meshes
#define GLE_NB_MATERIALS %nb_materials

uniform mat4 gle_CameraMatrix;
uniform mat4 gle_PMatrix;
uniform float gle_fogDensity;
#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	uniform vec3 gle_directionalLightDirection[GLE_NB_DIRECTIONAL_LIGHTS];
	uniform vec3 gle_directionalLightColor[GLE_NB_DIRECTIONAL_LIGHTS];
#endif
#if GLE_NB_POINT_LIGHTS > 0
	uniform vec3 gle_pointLightPosition[GLE_NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightAttenuation[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	uniform vec3 gle_spotLightPosition[GLE_NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightAttenuation[GLE_NB_SPOT_LIGHTS];
#endif

#if GLE_NB_STATIC_MESHES > 0
layout(std140) uniform staticMeshesBlock
{
	uniform mat4 MVMatrix[GLE_NB_STATIC_MESHES];
} gle_staticMeshes;
#endif

uniform mat4 gle_MVMatrix;
#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
	//uniform mat3 gle_NMatrix;
#endif

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

layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition; 
layout (location = GLE_IN_VERTEX_NORMAL_LOCATION) in vec3 gle_vNormal; 
layout (location = GLE_IN_VERTEX_TEXTURE_COORD_LOCATION) in vec2 gle_vTextureCoord;
layout (location = GLE_IN_VERTEX_MESH_ID_LOCATION) in vec3 gle_vMeshIdentifier;

out float gle_varying_fogFactor; 
out vec3 gle_varying_vLightWeighting; 
out vec2 gle_varying_vTextureCoord;
flat out vec3 gle_varying_vMeshIdentifier;

#if GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
	out vec3 gle_varying_normal;
	out vec3 gle_varying_eyeDirection;
#endif
#if GLE_NB_POINT_LIGHTS > 0
	out vec3 gle_varying_pointLightDirection[GLE_NB_POINT_LIGHTS];
	out float gle_varying_pointLightAttenuation[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	out vec3 gle_varying_spotLightDirection[GLE_NB_SPOT_LIGHTS];
	out float gle_varying_spotLightAttenuation[GLE_NB_SPOT_LIGHTS];
#endif

void main(void) {

	vec4 ambientColor = gle_material.materials[int(gle_vMeshIdentifier.z)].ambientColor;
	vec4 diffuseColor = gle_material.materials[int(gle_vMeshIdentifier.z)].diffuseColor;
	vec4 specularColor = gle_material.materials[int(gle_vMeshIdentifier.z)].specularColor;
	float shininess = gle_material.materials[int(gle_vMeshIdentifier.z)].shininess;
	float specularIntensity = gle_material.materials[int(gle_vMeshIdentifier.z)].specularIntensity;
	float diffuseIntensity = gle_material.materials[int(gle_vMeshIdentifier.z)].diffuseIntensity;
	
	gle_varying_vMeshIdentifier = gle_vMeshIdentifier;

	mat4 mvMatrix;
#if GLE_NB_STATIC_MESHES > 0
	if (!bool(gle_vMeshIdentifier.x))
		mvMatrix = gle_CameraMatrix * gle_staticMeshes.MVMatrix[int(gle_vMeshIdentifier.y)];
	else
#endif
		mvMatrix = gle_CameraMatrix * gle_MVMatrix;
	vec4 gle_mvPosition = mvMatrix * vec4(gle_vPosition, 1);
	mat3 nMatrix = transpose(mat3(inverse(mvMatrix)));
	gl_Position = gle_PMatrix * gle_mvPosition; 
	float fogDistance = length(gle_mvPosition); 
	const float LOG2 = 1.442695; 
	gle_varying_fogFactor = exp2(-gle_fogDensity * gle_fogDensity * fogDistance * fogDistance * LOG2); 
	gle_varying_fogFactor = clamp(gle_varying_fogFactor, 0.0, 1.0); 
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
		vec3 transformedNormal = normalize(nMatrix * gle_vNormal);
	#endif
	gle_varying_vTextureCoord = gle_vTextureCoord; 

	gle_varying_vLightWeighting = vec3(0.0, 0.0, 0.0);

	#if GLE_NB_DIRECTIONAL_LIGHTS > 0
		for (int i = 0; i < GLE_NB_DIRECTIONAL_LIGHTS; ++i)
		{
			float directionalLightWeighting = max(dot(transformedNormal, gle_directionalLightDirection[i]), 0.0);
			gle_varying_vLightWeighting += gle_directionalLightColor[i] * diffuseColor.rgb * directionalLightWeighting;
		}
	#endif
	#if GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
		gle_varying_normal = nMatrix * gle_vNormal;
		gle_varying_eyeDirection = -gle_mvPosition.xyz;
	#endif
	#if GLE_NB_POINT_LIGHTS > 0
		for (int i = 0; i < GLE_NB_POINT_LIGHTS; ++i)
		{
			vec3 pointLightDirection = gle_pointLightPosition[i] - gle_mvPosition.xyz;
			float d = length(pointLightDirection);
			if (gle_pointLightAttenuation[i].x == 0.0 && gle_pointLightAttenuation[i].y == 0.0 && gle_pointLightAttenuation[i].z == 0.0)
			   gle_varying_pointLightAttenuation[i] = 1.0;
			else
			   gle_varying_pointLightAttenuation[i] = 1.0 / (gle_pointLightAttenuation[i].x +
			   			      	 (gle_pointLightAttenuation[i].y * d) + 
							 (gle_pointLightAttenuation[i].z * d * d));
			if (gle_varying_pointLightAttenuation[i] > 1.0)
				gle_varying_pointLightAttenuation[i] = 1.0;
			gle_varying_pointLightDirection[i] = pointLightDirection;
		}
	#endif
	#if GLE_NB_SPOT_LIGHTS > 0
		for (int i = 0; i < GLE_NB_SPOT_LIGHTS; ++i)
		{
			vec3 spotLightDirection = gle_spotLightPosition[i] - gle_mvPosition.xyz;
			float d = length(spotLightDirection);
			if (gle_spotLightAttenuation[i].x == 0.0 && gle_spotLightAttenuation[i].y == 0.0 && gle_spotLightAttenuation[i].z == 0.0)
			   gle_varying_spotLightAttenuation[i] = 1.0;
			else
			   gle_varying_spotLightAttenuation[i] = 1.0 / (gle_spotLightAttenuation[i].x +
			   			      	 					(gle_spotLightAttenuation[i].y * d) + 
							 						(gle_spotLightAttenuation[i].z * d * d));
			if (gle_varying_spotLightAttenuation[i] > 1.0)
				gle_varying_spotLightAttenuation[i] = 1.0;
			gle_varying_spotLightDirection[i] = spotLightDirection;
		}
	#endif
	
	gle_varying_vLightWeighting += ambientColor.rgb;
}
