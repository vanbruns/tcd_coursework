#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 positionEye;
out vec3 normal;
out vec3 lightDirection;
out float attenuation;

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
	normal = normalize (normalEye);
	lightDirection = lightEye - positionEye;

	float lightDistance = length (lightDirection);

	lightDirection = lightDirection / lightDistance;
	attenuation = 1.0 / (attenuationConst
						+ attenuationLinear * lightDistance
						+ attenuationQuadratic * lightDistance * lightDistance);

	gl_Position =  projection * vec4 (positionEye, 1.0);
}