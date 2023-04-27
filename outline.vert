#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform float scale;
uniform float outline;

void main() {
	vec3 currPos = vec3(model * translation * -rotation * scale * vec4(aPos + aNormal * outline, 1.0));
	gl_Position = camMatrix * vec4(currPos, 1.0);
}
