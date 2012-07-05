#version 330 core 

#define GLE_IN_VERTEX_POSITION_LOCATION 0
#define GLE_IN_VERTEX_NORMAL_LOCATION 1
#define GLE_IN_VERTEX_TANGENT_LOCATION 2
#define GLE_IN_VERTEX_TEXTURE_COORD_LOCATION 3
#define GLE_IN_VERTEX_BONES_LOCATION 4
#define GLE_IN_VERTEX_MESH_ID_LOCATION 5

#define GLE_LIGHT_ENABLED 1

#define GLE_NB_DIRECTIONAL_LIGHTS %nb_directional_lights 
#define GLE_NB_POINT_LIGHTS %nb_point_lights
#define GLE_NB_SPOT_LIGHTS %nb_spot_lights
#define GLE_NB_STATIC_MESHES %nb_static_meshes
#define GLE_NB_MATERIALS %nb_materials
#define GLE_NB_BONES %nb_bones

#define GLE_CUBE_MAP 1

uniform float gle_fogDensity;

#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	uniform vec3 gle_directionalLightDirection[GLE_NB_DIRECTIONAL_LIGHTS];
#endif
#if GLE_NB_POINT_LIGHTS > 0
	uniform vec3 gle_pointLightPosition[GLE_NB_POINT_LIGHTS];
	uniform vec3 gle_pointLightAttenuation[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	uniform vec3 gle_spotLightPosition[GLE_NB_SPOT_LIGHTS];
	uniform vec3 gle_spotLightAttenuation[GLE_NB_SPOT_LIGHTS];
	uniform bool gle_spotLightHasShadowMap[GLE_NB_SPOT_LIGHTS];
	uniform mat4 gle_spotLightShadowMapMatrix[GLE_NB_SPOT_LIGHTS];
#endif
#if GLE_NB_BONES > 0
	uniform mat4 gle_bonesMatrix[GLE_NB_BONES];
#endif

uniform mat4 gle_MWMatrix;

uniform mat4 gle_ViewMatrix;
uniform mat4 gle_PMatrix;

#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
	uniform mat3 gle_NMatrix;

#if GLE_NB_STATIC_MESHES > 0

struct gle_StaticMesh {
	mat4	MWMatrix;
	float	skeletonIndex;
};

layout(std140) uniform gle_staticMeshesBlock
{
	gle_StaticMesh meshes[GLE_NB_STATIC_MESHES];
} gle_staticMeshes;

#endif

uniform vec3 gle_CameraPos;


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

layout(std140) uniform gle_materialBlock
{
	gle_MaterialStruct materials[GLE_NB_MATERIALS];
} gle_material;

layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition; 
layout (location = GLE_IN_VERTEX_NORMAL_LOCATION) in vec3 gle_vNormal;
layout (location = GLE_IN_VERTEX_TANGENT_LOCATION) in vec3 gle_vTangent; 
layout (location = GLE_IN_VERTEX_TEXTURE_COORD_LOCATION) in vec2 gle_vTextureCoord;
layout (location = GLE_IN_VERTEX_BONES_LOCATION) in vec4 gle_vBones;
layout (location = GLE_IN_VERTEX_MESH_ID_LOCATION) in vec3 gle_vMeshIdentifier;

out vec3 gle_varying_vPosition;
out float gle_varying_fogFactor; 
out vec3 gle_varying_vLightWeighting;
out vec2 gle_varying_vTextureCoord;
out vec3 gle_varying_envMapVector;

flat out vec3 gle_varying_vMeshIdentifier;

#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
	out vec3 gle_varying_normal;
	out vec3 gle_varying_eyeDirection;
#endif
#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	out vec3 gle_varying_directionalLightDirection[GLE_NB_DIRECTIONAL_LIGHTS];
#endif
#if GLE_NB_POINT_LIGHTS > 0
	out vec3 gle_varying_pointLightDirection[GLE_NB_POINT_LIGHTS];
	out float gle_varying_pointLightAttenuation[GLE_NB_POINT_LIGHTS];
#endif
#if GLE_NB_SPOT_LIGHTS > 0
	out vec3 gle_varying_spotLightDirection[GLE_NB_SPOT_LIGHTS];
	out vec3 gle_varying_spotLightRealDirection[GLE_NB_SPOT_LIGHTS];
	out float gle_varying_spotLightAttenuation[GLE_NB_SPOT_LIGHTS];
	out vec4 gle_varying_spotLightShadowMapCoord[GLE_NB_SPOT_LIGHTS];
#endif

void main(void) {

	vec4 ambientColor = gle_material.materials[int(gle_vMeshIdentifier.z)].ambientColor;
	vec4 diffuseColor = gle_material.materials[int(gle_vMeshIdentifier.z)].diffuseColor;
	vec4 specularColor = gle_material.materials[int(gle_vMeshIdentifier.z)].specularColor;
	float shininess = gle_material.materials[int(gle_vMeshIdentifier.z)].shininess;
	float specularIntensity = gle_material.materials[int(gle_vMeshIdentifier.z)].specularIntensity;
	float diffuseIntensity = gle_material.materials[int(gle_vMeshIdentifier.z)].diffuseIntensity;
	float reflectionIntensity = gle_material.materials[int(gle_vMeshIdentifier.z)].reflectionIntensity;
	float envMapType = gle_material.materials[int(gle_vMeshIdentifier.z)].envMapType;
	float hasColorMap = gle_material.materials[int(gle_vMeshIdentifier.z)].hasColorMap;
	float hasNormalMap = gle_material.materials[int(gle_vMeshIdentifier.z)].hasNormalMap;
	
	gle_varying_vMeshIdentifier = gle_vMeshIdentifier;
	
	mat4 mwMatrix;
	float skeletonIndex;
	#if GLE_NB_STATIC_MESHES > 0
		if (!bool(gle_vMeshIdentifier.x))
		{
			mwMatrix = gle_staticMeshes.meshes[int(gle_vMeshIdentifier.y)].MWMatrix;
			skeletonIndex = gle_staticMeshes.meshes[int(gle_vMeshIdentifier.y)].skeletonIndex;
		}
		else
	#endif
	{
			mwMatrix = gle_MWMatrix;
			skeletonIndex = 0;
	}
			
	gle_varying_vPosition = gle_vPosition;
	
	mat4 mvMatrix = gle_ViewMatrix * mwMatrix;
	vec4 gle_BonevPosition = vec4(gle_vPosition, 1.0);
	vec3 gle_BoneNormal = gle_vNormal;
	#if GLE_NB_BONES > 0
	if (skeletonIndex >= 0.0 && int(gle_vBones.x) >= 0)
	{
		gle_BonevPosition = gle_bonesMatrix[int(skeletonIndex) + int(gle_vBones.x)] * vec4(gle_vPosition, 1.0) * gle_vBones.y;
		gle_BoneNormal = mat3(gle_bonesMatrix[int(skeletonIndex) + int(gle_vBones.x)]) * gle_vNormal * gle_vBones.y;
		if (int(gle_vBones.z) >= 0)
		{
			gle_BonevPosition += gle_bonesMatrix[int(skeletonIndex) + int(gle_vBones.z)] * vec4(gle_vPosition, 1.0) * gle_vBones.w;
			gle_BoneNormal += mat3(gle_bonesMatrix[int(skeletonIndex) + int(gle_vBones.z)]) * gle_vNormal * gle_vBones.w;
		}
	}
	#endif
		
	vec4 gle_mvPosition = mvMatrix * gle_BonevPosition;
	mat3 nMatrix = transpose(mat3(inverse(mvMatrix)));
	gl_Position = gle_PMatrix * gle_mvPosition; 
	float fogDistance = length(gle_mvPosition); 
	const float LOG2 = 1.442695;
	gle_varying_fogFactor = exp2(-gle_fogDensity * gle_fogDensity * fogDistance * fogDistance * LOG2); 
	gle_varying_fogFactor = clamp(gle_varying_fogFactor, 0.0, 1.0); 
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
		vec3 transformedNormal = normalize(nMatrix * gle_BoneNormal);
	#endif
	gle_varying_vTextureCoord = gle_vTextureCoord;

	gle_varying_vLightWeighting = vec3(0.0, 0.0, 0.0);

	vec3 n, t, b;
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0 || GLE_NB_POINT_LIGHTS > 0 || GLE_NB_SPOT_LIGHTS > 0
		gle_varying_normal = nMatrix * gle_BoneNormal;
		if (hasNormalMap > 0.0)
		{
			n = transformedNormal;
			if (gle_vTangent.x != 0.0 && gle_vTangent.y != 0.0 && gle_vTangent.z != 0.0)
				t = normalize(nMatrix * gle_vTangent);
			else
				{
					vec3 c1 = cross(gle_BoneNormal, vec3(0.0, 0.0, 1.0)); 
					vec3 c2 = cross(gle_BoneNormal, vec3(0.0, 1.0, 0.0)); 
					if(length(c1) > length(c2))
						t = normalize(nMatrix * c1);
					else
						t = normalize(nMatrix * c2);
				}
			b = cross(n, t);
			vec3 tmp = -gle_mvPosition.xyz;
			gle_varying_eyeDirection.x = dot(tmp, t);
			gle_varying_eyeDirection.y = dot(tmp, b);
			gle_varying_eyeDirection.z = dot(tmp, n);
		}	
		else
			gle_varying_eyeDirection = -gle_mvPosition.xyz;

	#endif
	#if GLE_NB_DIRECTIONAL_LIGHTS > 0
	for (int i = 0; i < GLE_NB_DIRECTIONAL_LIGHTS; ++i)
	{
		vec3 directionalLightDirection = normalize(mat3(mvMatrix) * gle_directionalLightDirection[i]);
		if (hasNormalMap > 0.0)
			{
				vec3 tmp = directionalLightDirection;
				gle_varying_directionalLightDirection[i].x = dot(tmp, t);
				gle_varying_directionalLightDirection[i].y = dot(tmp, b);
				gle_varying_directionalLightDirection[i].z = dot(tmp, n);	
			}
		else
			gle_varying_directionalLightDirection[i] = directionalLightDirection;
	}
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
			if (hasNormalMap > 0.0)
			{
				vec3 tmp = pointLightDirection;
				gle_varying_pointLightDirection[i].x = dot(tmp, t);
				gle_varying_pointLightDirection[i].y = dot(tmp, b);
				gle_varying_pointLightDirection[i].z = dot(tmp, n);
			}
			else
				gle_varying_pointLightDirection[i] = pointLightDirection;
		}
	#endif
	#if GLE_NB_SPOT_LIGHTS > 0
		for (int i = 0; i < GLE_NB_SPOT_LIGHTS; ++i)
		{
			// Compute shadow map coords
			if (gle_spotLightHasShadowMap[i])
			{
				gle_varying_spotLightShadowMapCoord[i] =
					gle_spotLightShadowMapMatrix[i] * mwMatrix * vec4(gle_vPosition, 1.0);
			}
		
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
			if (hasNormalMap > 0.0)
			{
				vec3 tmp = spotLightDirection;
				gle_varying_spotLightDirection[i].x = dot(tmp, t);
				gle_varying_spotLightDirection[i].y = dot(tmp, b);
				gle_varying_spotLightDirection[i].z = dot(tmp, n);
			}
			else
				gle_varying_spotLightDirection[i] = spotLightDirection;
			gle_varying_spotLightRealDirection[i] = spotLightDirection;
		}
	#endif
	
	gle_varying_vLightWeighting += ambientColor.rgb;

	if (reflectionIntensity > 0 && envMapType == GLE_CUBE_MAP)
	{	
		vec4 WorldPos = mwMatrix * gle_BonevPosition;
		vec3 N = normalize(mat3(mwMatrix) * gle_BoneNormal);
		vec3 E = normalize(WorldPos.xyz - gle_CameraPos.xyz);
		gle_varying_envMapVector = reflect(E, N);
	}
}
