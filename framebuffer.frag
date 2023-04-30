#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform int isImpacting;
uniform float barrelPower;
uniform float iPressTime;
uniform int effectType;

const float offset_x = 1.0f / 800.0f;
const float offset_y = 1.0f / 800.0f;

vec2 offsets[9] = vec2[] 
(
    vec2(-offset_x,  offset_y), vec2(0.0f,    offset_y), vec2(offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2(0.0f,    0.0f),     vec2(offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2(0.0f,   -offset_y), vec2(offset_x, -offset_y)
);

float kernel[9] = float[] (
    1,  1,  1,
    1, -8,  1,    
    1,  1,  1
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

const float PI = 3.1415926535;

vec2 Distort(vec2 p, bool seeded = true)
{
    float theta  = atan(p.y, p.x);
    float radius = length(p);
	if(seeded) {
		radius = pow(radius, barrelPower * 2);
	} else {
		radius = pow(radius, 0);
	}
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

vec4 barrelDistort(bool seeded = true) {
	vec2 xy = 2.0 * TexCoords - 1.0;
	vec2 uv;
	float d = length(xy);
	if (d < 1.0)
	{
		uv = Distort(xy, seeded);
	}
	else
	{
		uv = TexCoords.xy;
	}
	vec4 c = texture2D(screenTexture, uv);

	return c;
}

vec4 getColor() {
	switch(effectType) {
		case 0:
			return (negativeView() - standardView()) * kernelView() + barrelDistort() - negativeView();
		case 1:
			return kernelView() * (barrelDistort() * negativeView());
		case 2:
			return (negativeView() - standardView()) * kernelView() + barrelDistort() +  negativeView();
		case 3:
			return (kernelView() / (negativeKernelView() - (barrelDistort() / negativeView())));
		case 4:
			return (kernelView() / (negativeKernelView() - (barrelDistort() + negativeView())));
		case 5:
			return negativeView() / (negativeKernelView() * barrelDistort());
	}
}

void main()
{ 
	if(iPressTime > 1.35f && isImpacting == 1) {
		FragColor = getColor();
		return;
	} 
	
	if (isImpacting == 1){
		FragColor = standardView() + barrelDistort();
		return;
	}
	 
	FragColor = standardView();
}