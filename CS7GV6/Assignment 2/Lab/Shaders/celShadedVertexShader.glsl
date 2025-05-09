#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 fragPos;
out vec3 vsNormal;
out vec3 vsPosition;

uniform mat4 model, projection, view;
uniform vec3 lightPos;

void main(){
	fragPos = vec3(model * vec4(vertexPosition, 1.0));
	vsNormal = normalize(mat3(model) * vertexNormal);
	vsPosition = mat3(model) * vertexPosition;
	gl_Position =  projection * view * model * vec4 (vertexPosition, 1.0);
}