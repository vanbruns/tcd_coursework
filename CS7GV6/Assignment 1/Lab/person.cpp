#include "person.h"

using namespace std;

void UpperArm::jointRotate(vec3 rotation) {
	Appendage::jointRotate(rotation);

	// Handle x-axis rotation validation
	if (vJointRotation.v[0] >= 360.0f) {
		vJointRotation.v[0] -= 360.0f;
	}

	if (vJointRotation.v[0] <= -360.0f) {
		vJointRotation.v[0] += 360.0f;
	}

	// Handle y-axis rotation validation
	if (vJointRotation.v[1] > 180.0f) {
		vJointRotation.v[1] = 180.0f;
	}

	if (vJointRotation.v[1] < 0.0f) {
		vJointRotation.v[1] = 0.0f;
	}

	// Handle z-axis rotation validation
	if (vJointRotation.v[2] > 90.0f) {
		vJointRotation.v[2] = 90.0f;
	}

	if (vJointRotation.v[2] < 0.0f) {
		vJointRotation.v[2] = 0.0f;
	}
}

UpperArm::UpperArm()
	: Appendage::Appendage() {

}

UpperArm::UpperArm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 shoulderLocation = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
	: Appendage::Appendage(buildingBlockMesh, side, Ball, shoulderLocation, scale) {

}

void LowerArm::jointRotate(vec3 rotation) {
	Appendage::jointRotate(rotation);

	// Handle x-axis rotation validation
	vJointRotation.v[0] = 0.0f;

	// Handle y-axis rotation validation
	if (vJointRotation.v[1] > 180.0f) {
		vJointRotation.v[1] = 180.0f;
	}

	if (vJointRotation.v[1] < 0.0f) {
		vJointRotation.v[1] = 0.0f;
	}

	// Handle z-axis rotation validation
	if (vJointRotation.v[2] > 80.0f) {
		vJointRotation.v[2] = 80.0f;
	}

	if (vJointRotation.v[2] < 0.0f) {
		vJointRotation.v[2] = 0.0f;
	}
}

LowerArm::LowerArm()
	: Appendage::Appendage() {
	
}

LowerArm::LowerArm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 centerLocation = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
	: Appendage::Appendage(buildingBlockMesh, side, Hinge, centerLocation, scale) {

	vJointLocation += vec3(direction * mesh->getHeight() / 2.0f, mesh->getLength() / 2.0f, 0.0f);
}

void Phalanx::jointRotate(vec3 rotation) {
	Appendage::jointRotate(rotation);

	// Handle x-axis rotation validation
	vJointRotation.v[0] = 0.0f;

	// Handle y-axis rotation validation
	vJointRotation.v[1] = 0.0f;

	// Handle z-axis rotation validation
	if (vJointRotation.v[2] > 90.0f) {
		vJointRotation.v[2] = 90.0f;
	}

	if (vJointRotation.v[2] < 0.0f) {
		vJointRotation.v[2] = 0.0f;
	}
}

Phalanx::Phalanx()
	: Appendage::Appendage() {
	
}

Phalanx::Phalanx(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 jointLocation = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
	: Appendage::Appendage(buildingBlockMesh, side, Hinge, jointLocation, scale) {

	vJointLocation.v[1] = vJointLocation.v[1] - mesh->getLength() / 2.0f;
}

void Hand::jointRotate(vec3 rotation) {
	Appendage::jointRotate(rotation);

	// Handle x-axis rotation validation
	if (vJointRotation.v[0] > 70.0f) {
		vJointRotation.v[0] = 70.0f;
	}

	if (vJointRotation.v[0] < -10.0f) {
		vJointRotation.v[0] = -10.0f;
	}

	// Handle y-axis rotation validation
	vJointRotation.v[1] = 0.0f;

	// Handle z-axis rotation validation
	if (vJointRotation.v[2] > 90.0f) {
		vJointRotation.v[2] = 90.0f;
	}

	if (vJointRotation.v[2] < -45.0f) {
		vJointRotation.v[2] = -45.0f;
	}
}

Hand::Hand()
	: Appendage::Appendage() {

}

Hand::Hand(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 jointLocation = vec3(0.0f, 0.0f, 0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
	: Appendage::Appendage(buildingBlockMesh, side, Hinge, jointLocation, scale, vec3(90.0f, 180.0f, 90.0f)) {

	vJointLocation.v[1] = vJointLocation.v[1] - mesh->getLength() / 2.0f;
}

void Arm::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 global = upperArm.draw(model, view, projection);

	global = lowerArm.draw(global, view, projection);
	//global = hand.draw(global, view, projection);
}

Arm::Arm() {

}

Arm::Arm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 location, vec3 scale) {
	mesh = buildingBlockMesh;
	upperArm = UpperArm(mesh, side, location, scale);
	lowerArm = LowerArm(mesh, side, vec3(location.v[0], location.v[1] - scale.v[1] * mesh->getLength(), location.v[2]), scale);
	//hand = Hand(mesh, side, vec3(location.v[0], location.v[1] - scale.v[1] * mesh->getLength() * 1.5f, location.v[2]), scale);
}

void Head::rotate(float pitch, float yaw) {
	vNeckRotation = vec3(pitch, yaw, 0.0f);
}

mat4 Head::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 orient = rotate_y_deg(identity_mat4(), -90.0f);

	orient = scale(orient, vScale);
	orient = translate(orient, vNeckLocation);

	mat4 local = identity_mat4();

	local = rotate_x_deg(local, vNeckRotation.v[0]);
	local = rotate_y_deg(local, vNeckRotation.v[1]);

	mat4 global = model * translateFirst(local, vNeckLocation);

	mesh->draw(global * orient, view, projection);

	return global;
}

Head::Head() {

}

Head::Head(GenericMesh* buildingBlockMesh, vec3 neckLocation, vec3 scale)
	: mesh(buildingBlockMesh), vNeckLocation(neckLocation), vScale(scale) {

}

void Body::rotate(float rotation) {
	facingAngle += rotation;

	if (facingAngle >= 360.0f) {
		facingAngle -= 360.0f;
	}

	if (facingAngle <= -360.0f) {
		facingAngle += 360.0f;
	}
}

mat4 Body::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 orient = rotate_x_deg(identity_mat4(), 90.0f);

	orient = rotate_z_deg(orient, 90.0f);

	orient = scale(orient, bScale);
	orient = translate(orient, location);

	mat4 local = identity_mat4();

	local = rotate_y_deg(local, facingAngle);

	mat4 global = model * translateFirst(local, location);

	mesh->draw(global * orient, view, projection);

	return global;
}

Body::Body() {

}

Body::Body(GenericMesh* buildingBlockMesh, vec3 bodyLocation, vec3 scale)
	: mesh(buildingBlockMesh), location(bodyLocation), bScale(scale) {

}

void Person::draw(mat4 model, mat4 view, mat4 projection) {
	mat4 global = body.draw(model, view, projection);

	head.draw(global, view, projection);
	leftArm.draw(global, view, projection);
	rightArm.draw(global, view, projection);
}

Person::Person() {

}

Person::Person(GenericMesh* buildingBlockMesh, vec3 location, vec3 scale) {
	mesh = buildingBlockMesh;
	body = Body(mesh, location, scale * 5.0f);
	head = Head(mesh, vec3(location.v[0], location.v[1] + scale.v[1] * 5.0f * mesh->getLength() / 4.0f, location.v[2] + scale.v[2] * 5.0f * mesh->getHeight() / 3.0f), scale * 1.5f);
	leftArm = Arm(mesh, Left, vec3(location.v[0] - scale.v[0] * 5.0f * mesh->getWidth() / 2.0f, location.v[1], location.v[2]), vec3(scale.v[0] * 0.5f, scale.v[1] * 1.5f, scale.v[2] * 1.0f));
	rightArm = Arm(mesh, Right, vec3(location.v[0] + scale.v[0] * 5.0f * mesh->getWidth() / 2.0f, location.v[1], location.v[2]), vec3(scale.v[0] * 0.5f, scale.v[1] * 1.5f, scale.v[2] * 1.0f));
}