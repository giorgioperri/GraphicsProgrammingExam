#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 currPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;

// If u don't use uniforms OpenGL will delete them automatically and that may cause errors
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform float scale;


void main() {
	currPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));

	normal = aNormal;

	color = aColor;
	
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTexCoord;

	gl_Position = camMatrix * vec4(currPos, 1.0f);
}
