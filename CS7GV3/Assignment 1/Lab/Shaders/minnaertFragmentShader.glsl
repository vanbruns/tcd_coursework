#version 330

in vec3 positionEye;
in vec3 surfaceNormal;
in vec3 lightVector;
in float fatt;

out vec4 fragColour;

uniform vec3 lightDiffuse;

uniform vec3 reflectDiffuse;

uniform float k;

void main () {
	float diffuse1 = dot (lightVector, surfaceNormal);
	float diffuse2 = dot (surfaceNormal, normalize (positionEye));

	diffuse1 = pow (diffuse1, k + 1.0);
	diffuse2 = pow (diffuse2, 1.0 - k);

	vec3 I = lightDiffuse * reflectDiffuse * fatt * diffuse1 * diffuse2;

	// final colour
	fragColour = vec4 (I, 1.0);
}