#version 330

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 normal_eye;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main(){
	normal_eye = (view * vec4 (vertex_normal, 0.0)).xyz;
	gl_Position =  projection * view * model * vec4 (vertex_position, 1.0);
}