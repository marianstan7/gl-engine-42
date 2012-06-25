#version 330 core
#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0

layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor;

uniform vec3 gle_color;

in vec3 pos;

void main(void) {

gle_FragColor = vec4(gle_color, 1.0);

}
