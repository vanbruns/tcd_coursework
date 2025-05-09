#include "maths_funcs.h"

#include <string>
#include <GL/glew.h>

using namespace std;

class Shader {
private:
	GLuint shaderID;
	string vertexShader;
	string fragmentShader;

	static string readShaderSource(string filename);
	void addShader(string shaderText, GLenum shaderType);

public:
	GLuint getShaderID() {
		return shaderID;
	}

	GLuint compile();
	Shader();
	Shader(string vertexShaderFilename, string fragmentShaderFilename);
};