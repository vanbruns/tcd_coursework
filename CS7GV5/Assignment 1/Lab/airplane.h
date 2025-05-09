#include "meshes.h"

using namespace std;

class Propeller {
public:
	void rotate(float degrees);
	mat4 draw(mat4 model, mat4 view, mat4 projection);
	void init(Shader* shader, LightSource* lightSource);
	Propeller();
	Propeller(vec3 axilCenterLocation);
private:
	GenericMesh* mesh;
	float fRotation = 0.0f;
	vec3 vAxleCenterLocation;
};

class Wheels {
public:
	void rotate(float degrees);
	mat4 draw(mat4 model, mat4 view, mat4 projection);
	void init(Shader* shader, LightSource* lightSource);
	Wheels();
	Wheels(vec3 axilCenterLocation);
private:
	GenericMesh* mesh;
	float fRotation = 0.0f;
	vec3 vAxleCenterLocation;
};

class Airplane {
public:
	void propellerRotate(float rotation) {
		propeller.rotate(rotation);
	}

	void wheelsRotate(float rotation) {
		wheels.rotate(rotation);
	}

	void draw(mat4 model, mat4 view, mat4 projection);
	void init(Shader* shader, LightSource* lightSource);
	Airplane();
private:
	GenericMesh* mesh;
	Propeller propeller;
	Wheels wheels;
};