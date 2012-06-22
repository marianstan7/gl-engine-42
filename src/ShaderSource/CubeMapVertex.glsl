#version 330 core

#define GLE_IN_VERTEX_POSITION_LOCATION 0

layout (location = GLE_IN_VERTEX_POSITION_LOCATION) in vec3 gle_vPosition;

uniform	   mat4 gle_MVMatrix;
uniform mat4 gle_PMatrix;
uniform vec3 gle_CameraPos;

out vec3 pos;

void main(void) {

vec4 gle_mvPosition	= gle_MVMatrix * vec4(gle_vPosition + gle_CameraPos, 1);
gl_Position		= (gle_PMatrix * gle_mvPosition);
pos			= gle_vPosition;

}
