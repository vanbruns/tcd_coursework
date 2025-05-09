#version 330

in vec3 positionEye;
in vec3 surfaceNormal;
in vec3 lightVector;
in float fatt;

out vec4 fragColour;

uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 lightAmbient;

uniform vec3 reflectDiffuse;
uniform vec3 reflectSpecular;
uniform vec3 reflectAmbient;

uniform float specularExponent;

void main () {
	// ambient intensity
	vec3 Ia = lightAmbient * reflectAmbient;

	float diffuse = dot (surfaceNormal, lightVector);

	// diffuse intensity
	vec3 Id = lightDiffuse * reflectDiffuse * fatt * diffuse;

	vec3 reflectionVector = 2.0 * dot (lightVector, surfaceNormal) * surfaceNormal - lightVector;
	float specular = dot (normalize (positionEye), reflectionVector);

	specular = pow (specular, specularExponent / 50.0);

	// specular intensity
	vec3 Is = lightSpecular * reflectSpecular * fatt * specular;

	// final colour
	fragColour = vec4 (Ia + Id + Is, 1.0);
}