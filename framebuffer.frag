#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset_x = 1.0f / 800.0f;
const float offset_y = 1.0f / 800.0f;

vec2 offsets[9] = vec2[] 
(
    vec2(-offset_x,  offset_y), vec2(0.0f,    offset_y), vec2(offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2(0.0f,    0.0f),     vec2(offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f,   -offset_y), vec2(offset_x, -offset_y)
);

float kernel[9] = float[] (
    1, 1, 1,
    1, -8, 1,    
    1, 1, 1
);

vec4 kernelView() {
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++) {
		color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
	}

    return vec4(color, 1.0);
}

vec4 negativeKernelView() {
	vec3 color = vec3(0.0);
	for(int i = 0; i < 9; i++) {
		color += vec3(texture(screenTexture, TexCoords.st + offsets[i])) * kernel[i];
	}
	return vec4(1.0f) - vec4(color, 1.0);
}

vec4 blurView() {
	vec3 color = vec3(0.0);
	for(int i = 0; i < 9; i++) {
		color += vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}
	return vec4(color / 9.0, 1.0);
}

vec4 standardView() {
	return texture(screenTexture, TexCoords);
}

vec4 grayscaleView() {
	vec4 tex = texture(screenTexture, TexCoords);
	float avg = (tex.r + tex.g + tex.b) / 3.0;
	return vec4(avg, avg, avg, 1.0);
}

vec4 negativeView() {
	return vec4(1.0f) - texture(screenTexture, TexCoords);
}


void main()
{ 
    FragColor = standardView();
}