#version 330 core

out vec4 FragColor;
uniform float iPressTime;

void main()  {

	if(iPressTime > 1.35f) {
		FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}

	FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}