#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 positionEye;
out vec3 surfaceNormal;
out vec3 lightVector;
out float fatt;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

uniform vec3 lightPosition;
uniform float attenuationConst;
uniform float attenuationLinear;
uniform float attenuationQuadratic;

void main(){
	vec3 normalEye = vec3 (view * model * vec4 (vertexNormal, 0.0));
	vec3 lightEye = vec3 (view * vec4 (lightPosition, 1.0));
	positionEye = vec3 (view * model * vec4 (vertexPosition, 1.0));
	surfaceNormal = normalize (normalEye);
	lightVector = lightEye - positionEye;

	float lightDistance = length (lightVector);

	lightVector = lightVector / lightDistance;
	fatt = 1.0 / (attenuationConst
						+ attenuationLinear * lightDistance
						+ attenuationQuadratic * lightDistance * lightDistance);
	fatt = min (fatt, 1);

	gl_Position =  projection * vec4 (positionEye, 1.0);
}