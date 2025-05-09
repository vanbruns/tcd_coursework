#include "airplane.h"

using namespace std;

void Propeller::rotate(float degrees) {
	fRotation += degrees;

	if (fRotation >= 360.0f) {
		fRotation -= 360.0f;
	}

	if (fRotation <= -360.0f) {
		fRotation += 360.0f;
	}
}

mat4 Propeller::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 orient = identity_mat4();

	orient = translate(orient, vAxleCenterLocation);

	mat4 local = identity_mat4();

	local = rotate_z_deg(local, fRotation);

	mat4 global = model * translateFirst(local, vAxleCenterLocation);

	mesh->draw(global * orient, view, projection);

	return global;
}

void Propeller::init(Shader* shader, LightSource* lightSource) {
	mesh->init(shader, lightSource);
}

Propeller::Propeller() {

}

Propeller::Propeller(vec3 axleCenterLocation)
	: mesh(new PropellerMesh()), vAxleCenterLocation(axleCenterLocation) {

}

void Wheels::rotate(float degrees) {
	fRotation += degrees;

	if (fRotation >= 360.0f) {
		fRotation -= 360.0f;
	}

	if (fRotation <= -360.0f) {
		fRotation += 360.0f;
	}
}

mat4 Wheels::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 orient = identity_mat4();

	orient = translate(orient, vAxleCenterLocation);

	mat4 local = identity_mat4();

	local = rotate_x_deg(local, fRotation);

	mat4 global = model * translateFirst(local, vAxleCenterLocation);

	mesh->draw(global * orient, view, projection);

	return global;
}

void Wheels::init(Shader* shader, LightSource* lightSource) {
	mesh->init(shader, lightSource);
}

Wheels::Wheels() {

}

Wheels::Wheels(vec3 axleCenterLocation)
	: mesh(new WheelsMesh()), vAxleCenterLocation(axleCenterLocation) {

}

void Airplane::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 global = model;

	mesh->draw(global, view, projection);

	propeller.draw(global, view, projection);
	wheels.draw(global, view, projection);
}

void Airplane::init(Shader* shader, LightSource* lightSource) {
	mesh->init(shader, lightSource);
	propeller.init(shader, lightSource);
	wheels.init(shader, lightSource);
}

Airplane::Airplane()
	: mesh(new AirplaneMesh()) {
	propeller = Propeller(vec3(0.0f, -24.0f, 133.0f));
	wheels = Wheels(vec3(0.0f, -112.0f, 64.0f));
}