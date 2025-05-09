#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

string Shader::readShaderSource(string filename) {
	ifstream file(filename.data());

	if (file.fail()) {
		cout << "error loading shader called " << filename;
		exit(1);
	}

	stringstream stream;
	stream << file.rdbuf();
	file.close();

	return stream.str();
}

void Shader::addShader(string shaderText, GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);

	if (shaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(0);
	}

	string outShader = Shader::readShaderSource(shaderText);
	const char* pShaderSource = outShader.data();

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(shaderObj, 1, (const GLchar * *)& pShaderSource, NULL);

	// compile the shader and check for errors
	glCompileShader(shaderObj);

	GLint success;

	// check for shader related errors using glGetShaderiv
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLchar infoLog[1024];

		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}

	// Attach the compiled shader object to the program object
	glAttachShader(shaderID, shaderObj);
}

GLuint Shader::compile() {
	// Create two shader objects, one for the vertex, and one for the fragment shader
	addShader(vertexShader, GL_VERTEX_SHADER);
	addShader(fragmentShader, GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };


	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderID);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

	if (success == 0) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderID);

	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderID, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}

	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderID);

	return shaderID;
}

Shader::Shader() :shaderID(0) {

}

Shader::Shader(string vertexShaderFilename, string fragmentShaderFilename)
	: vertexShader (vertexShaderFilename), fragmentShader (fragmentShaderFilename) {
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderID = glCreateProgram();

	if (shaderID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}
}