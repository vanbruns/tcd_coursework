#version 330

in vec3 normal_eye;

out vec4 fragment_colour;

void main(){
	fragment_colour = vec4 (normal_eye, 1.0);
}