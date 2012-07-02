#version 330 core

#define GLE_NB_STATIC_MESHES %nb_static_meshes

#define GLE_IN_VERTEX_POSITION_LOCATION 0
#define GLE_IN_VERTEX_MESH_ID_LOCATION 5

layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition;
layout (location = GLE_IN_VERTEX_MESH_ID_LOCATION) in vec3 gle_vMeshIdentifier;

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

uniform mat4 gle_MWMatrix;

uniform mat4 gle_ViewMatrix;
uniform mat4 gle_PMatrix;

void main(void) {

	mat4 mwMatrix;
	#if GLE_NB_STATIC_MESHES > 0
		if (!bool(gle_vMeshIdentifier.x))
			mwMatrix = gle_staticMeshes.meshes[int(gle_vMeshIdentifier.y)].MWMatrix;
		else
	#endif
			mwMatrix = gle_MWMatrix;
	
	mat4 mvMatrix = gle_PMatrix * gle_ViewMatrix * mwMatrix;

	gl_Position			= mvMatrix * vec4(gle_vPosition, 1);
	
}
