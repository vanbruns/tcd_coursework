#include "shader.h"

class LightSource {
private:
	int lightPositionLocation = 0;
	int lightDiffuseLocation = 0;
	int lightSpecularLocation = 0;
	int lightAmbientLocation = 0;
	int attenuationConstLocation = 0;
	int attenuationLinearLocation = 0;
	int attenuationQuadraticLocation = 0;

	vec3 vLightPosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 vLightDiffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 vLightSpecular = vec3(0.0f, 0.0f, 0.0f);
	vec3 vLightAmbient = vec3(0.0f, 0.0f, 0.0f);

	float fAttenuationConst = 0.0f;
	float fAttenuationLinear = 0.0f;
	float fAttenuationQuadratic = 0.0f;
public:
	void setUniform() {
		glUniform3fv(lightPositionLocation, 1, vLightPosition.v);
		glUniform3fv(lightDiffuseLocation, 1, vLightDiffuse.v);
		glUniform3fv(lightSpecularLocation, 1, vLightSpecular.v);
		glUniform3fv(lightAmbientLocation, 1, vLightAmbient.v);
		glUniform1f(attenuationConstLocation, fAttenuationConst);
		glUniform1f(attenuationLinearLocation, fAttenuationLinear);
		glUniform1f(attenuationQuadraticLocation, fAttenuationQuadratic);
	}

	void init(Shader* shader) {
		lightPositionLocation = glGetUniformLocation(shader->getShaderID(), "lightPosition");
		lightDiffuseLocation = glGetUniformLocation(shader->getShaderID(), "lightDiffuse");
		lightSpecularLocation = glGetUniformLocation(shader->getShaderID(), "lightSpecular");
		lightAmbientLocation = glGetUniformLocation(shader->getShaderID(), "lightAmbient");
		attenuationConstLocation = glGetUniformLocation(shader->getShaderID(), "attenuationConst");
		attenuationLinearLocation = glGetUniformLocation(shader->getShaderID(), "attenuationLinear");
		attenuationQuadraticLocation = glGetUniformLocation(shader->getShaderID(), "attenuationQuadratic");
	}

	void setLightPosition(vec3 lightPosition) {
		vLightPosition = lightPosition;
	}

	LightSource() {

	}

	LightSource(vec3 lightPosition, vec3 lightDiffuse, vec3 lightSpecular, vec3 lightAmbient, float attenuationConst, float attenuationLinear, float attenuationQuaratic) 
		: vLightPosition(lightPosition), vLightDiffuse(lightDiffuse), vLightSpecular(lightSpecular), vLightAmbient(lightAmbient),
		fAttenuationConst(attenuationConst), fAttenuationLinear(attenuationLinear), fAttenuationQuadratic(attenuationQuaratic) {

	}
};

class GenericMesh {
private:
	int projectionMatrixLocation = 0;
	int viewMatrixLocation = 0;
	int modelMatrixLocation = 0;
	int reflectDiffuseLocation = 0;
	int reflectSpecularLocation = 0;
	int reflectAmbientLocation = 0;
	int spotSizeLocation = 0;
	int shininessLocation = 0;
protected:
	Shader* pShader;
	LightSource* plightSource;
	unsigned int vao = 0;
	int vertexCount;
	float* vertices;
	float* normals;
	bool ignoreSurfaceAndLighting = false;

	vec3 reflectDiffuse;
	vec3 reflectSpecular;
	vec3 reflectAmbient;
	float spotSize;
	float shininess;

	void generateObjectBuffer() {
		GLuint vp_vbo = 0;
		GLuint vertPosLoc = glGetAttribLocation(pShader->getShaderID(), "vertexPosition");
		GLuint vertNormLoc = glGetAttribLocation(pShader->getShaderID(), "vertexNormal");

		glGenBuffers(1, &vp_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

		GLuint vn_vbo = 0;

		glGenBuffers(1, &vn_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * vertexCount * sizeof(float), normals, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(vertPosLoc);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
		glVertexAttribPointer(vertPosLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vertNormLoc);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
		glVertexAttribPointer(vertNormLoc, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindVertexArray(0);
	}
public:
	virtual float getLength() {
		return 1.0f;
	}

	virtual float getWidth() {
		return 1.0f;
	}

	virtual float getHeight() {
		return 1.0f;
	}

	void draw(mat4 model, mat4 view, mat4 projection) {
		glUseProgram(pShader->getShaderID());

		glBindVertexArray(vao);

		if (!ignoreSurfaceAndLighting) {
			plightSource->setUniform();

			glUniform3fv(reflectDiffuseLocation, 1, reflectDiffuse.v);
			glUniform3fv(reflectSpecularLocation, 1, reflectSpecular.v);
			glUniform3fv(reflectAmbientLocation, 1, reflectAmbient.v);
			glUniform1f(spotSizeLocation, spotSize);
			glUniform1f(shininessLocation, shininess);
		}

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projection.m);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, view.m);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, model.m);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glBindVertexArray(0);
	}

	virtual void init(Shader* shader, LightSource* lightSource) {
		pShader = shader;
		plightSource = lightSource;

		if (!ignoreSurfaceAndLighting) {
			plightSource->init(pShader);

			reflectDiffuseLocation = glGetUniformLocation(pShader->getShaderID(), "reflectDiffuse");
			reflectSpecularLocation = glGetUniformLocation(pShader->getShaderID(), "reflectSpecular");
			reflectAmbientLocation = glGetUniformLocation(pShader->getShaderID(), "reflectAmbient");
			spotSizeLocation = glGetUniformLocation(pShader->getShaderID(), "spotSize");
			shininessLocation = glGetUniformLocation(pShader->getShaderID(), "shininess");
		}

		modelMatrixLocation = glGetUniformLocation(pShader->getShaderID(), "model");
		viewMatrixLocation = glGetUniformLocation(pShader->getShaderID(), "view");
		projectionMatrixLocation = glGetUniformLocation(pShader->getShaderID(), "projection");

		generateObjectBuffer();
	}
};

class Teapot : public GenericMesh {
public:
	float getLength() {
		return 2 * 16.0703f;
	}

	float getWidth() {
		return 2 * 10.0f;
	}

	float getHeight() {
		return 2 * 7.875f;
	}

	void init(Shader* shader, LightSource* lightSource);
};

class Cylinder : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class IcoSphere : public GenericMesh {
public:
	float getLength() {
		return 2 * 1.0f;
	}

	float getWidth() {
		return 2 * 1.0f;
	}

	float getHeight() {
		return 2 * 1.0f;
	}

	void init(Shader* shader, LightSource* lightSource);
};

class Cone : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Cube : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Dodecahedron : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Icosahedron : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Octahedron : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Tetrahedron : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Torus : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Merkaba : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};

class Table : public GenericMesh {
public:
	void init(Shader* shader, LightSource* lightSource);
};