#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

// If u don't use uniforms OpenGL will delete them automatically and that may cause errors
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(aPos, 1.0f);

	color = aColor;
	
	texCoord = aTexCoord;
}
