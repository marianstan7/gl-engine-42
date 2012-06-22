#version 330 core
#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0

layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor;

uniform samplerCube gle_cubeMap;

in vec3 pos;

void main(void) {

gle_FragColor = vec4(texture(gle_cubeMap, pos).rgb, 1.0);

}