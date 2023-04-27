#version 330 core

out vec4 FragColor;

in vec3 currPos;
in vec3 normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight() {
	vec3 lightVec = lightPos - currPos;
	float dist = length(lightVec);

	float a = 3.0;
	float b = 0.7;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0);

	float ambient = 0.2f;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	float specular = specularLight * specAmount;

    return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight() {
	float ambient = 0.2f;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 16);
	float specular = specularLight * specAmount;
	
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight() {
	float outerCone = 0.9f;
	float innerCone = 0.95f;

	float ambient = 0.2f;

	vec3 normal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currPos);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
	float specular = specularLight * specAmount;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

float near = 0.1f;
float far = 20.0f;

float linearizeDepth(float depth) {
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth) {
	float steepness = 3;
	float offset = 3;
	float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main(){
    //FragColor = directionalLight();
	//FragColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.0);

	//Fog effect + directional light
//	float depth = logisticDepth(gl_FragCoord.z);
//	FragColor = directionalLight() * (1.0f - depth) + vec4(depth * vec3(0.07f, 0.13f, 0.17f), 1.0f);

	FragColor = directionalLight();
}