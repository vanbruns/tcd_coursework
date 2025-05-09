#version 330

in vec3 normal;
in vec3 lightDirection;
in vec3 halfVector;
in float attenuation;

out vec4 fragColour;

uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightAmbient;

uniform vec3 reflectDiffuse;
uniform vec3 reflectSpecular;
uniform vec3 reflectAmbient;

uniform float shininess;
uniform float strength;

void main () {
	float diffuse = max (0.0, dot (normal, lightDirection));
	float specular = max (0.0, dot (normal, halfVector));

	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow (specular, shininess) * strength;

	vec3 scatteredLight = lightAmbient * reflectAmbient + lightDiffuse * reflectDiffuse * diffuse;
	vec3 reflectedLight = lightSpecular * reflectSpecular * specular * attenuation;

	fragColour = vec4 (min (scatteredLight + reflectedLight, vec3 (1.0)), 1.0);
}