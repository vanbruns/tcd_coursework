#version 330

in vec3 positionEye;
in vec3 normal;
in vec3 lightDirection;
in float attenuation;

out vec4 fragColour;

uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightAmbient;

uniform vec3 reflectDiffuse;
uniform vec3 reflectSpecular;
uniform vec3 reflectAmbient;

uniform float spotSize;
uniform float shininess;

void main () {
	// ambient intensity
	vec3 Ia = lightAmbient * reflectAmbient;

	float diffuse = max (dot (lightDirection, normal), 0.0);

	// diffuse intensity
	vec3 Id = lightDiffuse * reflectDiffuse * diffuse;

	vec3 reflectionEye = reflect (-lightDirection, normal);
	float specular = dot (reflectionEye, normalize (-positionEye));
	specular = clamp (specular, 0.0, 1.0);
	specular = pow (specular, spotSize);

	// specular intensity
	vec3 Is = lightSpecular * reflectSpecular * specular * shininess / attenuation;

	// final colour
	fragColour = vec4 (Is + Id + Ia, 1.0);
}