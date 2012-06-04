#version 330 core 
  
#define GLE_OUT_FRAGMENT_COLOR_LOCATION 0 
  
layout (location = GLE_OUT_FRAGMENT_COLOR_LOCATION) out vec4 gle_FragColor; 

uniform vec3 gle_fogColor;

uniform sampler2D gle_colorMap;
uniform bool gle_hasColorMap;

in float gle_varying_fogFactor; 
in vec3 gle_varying_vLightWeighting;
in float gle_varying_vLightAttenuation;
in vec2 gle_varying_vTextureCoord; 

void main(void) { 
	vec2 tmp = vec2(gle_varying_vTextureCoord.x, -gle_varying_vTextureCoord.y);
	if (gle_hasColorMap)
	{
		gle_FragColor = texture2D(gle_colorMap, tmp);
	}
	else
	{
		gle_FragColor = vec4(1.0);
	}
	if (gle_varying_vLightAttenuation > 1.0)
	   gle_FragColor *= vec4(gle_varying_vLightWeighting, 1.0);
	else
	   gle_FragColor *= vec4(gle_varying_vLightWeighting * gle_varying_vLightAttenuation, 1.0);
	gle_FragColor = mix(vec4(gle_fogColor, 1.0), gle_FragColor, gle_varying_fogFactor);
}
