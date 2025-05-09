#version 330

in vec3 fragPos;
in vec3 vsNormal;
in vec3 vsPosition;

out vec4 fragColour;

uniform vec3 lightPosition;

void main() {
	// Global Lighting Variables
	vec3 lightDir = normalize(lightPosition - fragPos);
	vec3 norm = normalize(vsNormal);
	float intensity;
	vec4 color;

	intensity = dot(lightDir, normalize(norm));

	if (intensity > 0.5) {
		color = vec4(0.686, 0.933, 0.933, 1.0); 
	}
	else {
		color = vec4(0.251, 0.878, 0.816, 1.0); 
	}

	fragColour = color;
}