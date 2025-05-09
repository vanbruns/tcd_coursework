#include "meshes.h"

using namespace std;

enum SideOfBody { Left, Right };
enum JointType { Ball, Hinge };

class Appendage {
public:
	void jointRotate(vec3 rotation) {
		vJointRotation += rotation;
	}

	mat4 draw(mat4 model, mat4 view, mat4 projection) {
		mat4 orient = rotate_z_deg(identity_mat4(), vOrientRotation.v[2]);

		if (direction > 0) {
			orient = rotate_y_deg(orient, vOrientRotation.v[1]);
		}

		orient = rotate_x_deg(orient, vOrientRotation.v[0]);
		orient = scale(orient, vScale);
		orient = translate(orient, vOrientLocation);

		mat4 local = identity_mat4();

		local = rotate_y_deg(local, direction * joint * vJointRotation.v[1]);
		local = rotate_x_deg(local, -vJointRotation.v[0]);
		local = rotate_z_deg(local, direction * joint * vJointRotation.v[2]);

		mat4 global = model * translateFirst(local, vJointLocation);

		mesh->draw(global * orient, view, projection);

		return global;
	}
protected:
	GenericMesh* mesh = NULL;
	vec3 vJointRotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 vOrientRotation;
	vec3 vOrientLocation;
	vec3 vJointLocation;
	vec3 vScale;
	int direction = 1;
	int joint = 1;

	Appendage() {

	}

	Appendage(GenericMesh* buildingBlockMesh, SideOfBody side, JointType jointType, vec3 jointLocation, vec3 scale)
		: Appendage(buildingBlockMesh, side, jointType, jointLocation, scale, vec3(0.0f, 180.0f, 90.0f), jointLocation) {

	}

	Appendage(GenericMesh* buildingBlockMesh, SideOfBody side, JointType jointType, vec3 jointLocation, vec3 scale, vec3 orientRotation)
		: Appendage(buildingBlockMesh, side, jointType, jointLocation, scale, orientRotation, jointLocation) {
		
	}

	Appendage(GenericMesh* buildingBlockMesh, SideOfBody side, JointType jointType, vec3 jointLocation, vec3 scale, vec3 orientRotation, vec3 orientLocation) {
		mesh = buildingBlockMesh;
		vJointLocation = jointLocation;
		vScale = scale;
		vOrientRotation = orientRotation;
		vOrientLocation = orientLocation;

		switch (side) {
			case Left:
				direction = -1;
				break;
			case Right:
				direction = 1;
				break;
		}

		switch (jointType) {
			case Ball:
				joint = 1;
				break;
			case Hinge:
				joint = -1;
				break;
		}
	}
};

class UpperArm : public Appendage {
public:
	void jointRotate(vec3 rotation);
	UpperArm();
	UpperArm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 shoulderLocation, vec3 scale);
};

class LowerArm : public Appendage {
public:
	void jointRotate(vec3 rotation);
	LowerArm();
	LowerArm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 centerLocation, vec3 scale);
};

class Phalanx : public Appendage {
public:
	void jointRotate(vec3 rotation);
	Phalanx();
	Phalanx(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 elbowLocation, vec3 scale);
};

class Finger {

};

class Hand : public Appendage {
public:
	void grabObject(GenericMesh* object) {
		heldObject = object;
	}

	void releaseObject() {
		heldObject = NULL;
	}

	mat4 draw(mat4 model, mat4 view, mat4 projection) {
		mat4 global = Appendage::draw(model, view, projection);

		return global;
	}

	void jointRotate(vec3 rotation);
	Hand();
	Hand(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 elbowLocation, vec3 scale);
private:
	GenericMesh* heldObject = NULL;
	Phalanx indexPhalanx;
	Phalanx middlePhalanx;
	Phalanx ringPhalanx;
	Phalanx pinkyPhalanx;
};

class Arm {
public:
	void forwardShoulderRotate(float rotation) {
		upperArm.jointRotate(vec3(rotation, 0.0f, 0.0f));
	}

	void sideShoulderRotate(float rotation) {
		upperArm.jointRotate(vec3(0.0f, 0.0f, rotation));
	}

	void upperArmRotate(float rotation) {
		upperArm.jointRotate(vec3(0.0f, rotation, 0.0f));
	}

	void elbowRotate(float rotation) {
		lowerArm.jointRotate(vec3(0.0f, 0.0f, rotation));
	}

	void lowerArmRotate(float rotation) {
		lowerArm.jointRotate(vec3(0.0f, rotation, 0.0f));
	}

	void wristRotate(float rotation) {
		hand.jointRotate(vec3(0.0f, rotation, 0.0f));
	}

	void draw(mat4 model, mat4 view, mat4 projection);
	Arm();
	Arm(GenericMesh* buildingBlockMesh, SideOfBody side, vec3 location, vec3 scale);
private:
	GenericMesh* mesh;
	UpperArm upperArm;
	LowerArm lowerArm;
	Hand hand;
};


class Head {
public:
	void rotate(float pitch, float yaw);
	mat4 draw(mat4 model, mat4 view, mat4 projection);
	Head();
	Head(GenericMesh* buildingBlockMesh, vec3 neckLocation, vec3 scale);
private:
	GenericMesh* mesh;
	vec3 vNeckRotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 vNeckLocation;
	vec3 vScale;
};

class Body {
public:
	void rotate(float rotation);
	mat4 draw(mat4 model, mat4 view, mat4 projection);
	Body();
	Body(GenericMesh* buildingBlockMesh, vec3 bodyLocation, vec3 scale);
private:
	GenericMesh* mesh;
	float facingAngle = 0.0f;
	vec3 location;
	vec3 bScale;
};

class Person {
public:
	void forwardLeftShoulderRotate(float rotation) {
		leftArm.forwardShoulderRotate(rotation);
	}

	void forwardRightShoulderRotate(float rotation) {
		rightArm.forwardShoulderRotate(rotation);
	}

	void sideLeftShoulderRotate(float rotation) {
		leftArm.sideShoulderRotate(rotation);
	}

	void sideRightShoulderRotate(float rotation) {
		rightArm.sideShoulderRotate(rotation);
	}

	void upperLeftArmRotate(float rotation) {
		leftArm.upperArmRotate(rotation);
	}

	void upperRightArmRotate(float rotation) {
		rightArm.upperArmRotate(rotation);
	}

	void leftElbowRotate(float rotation) {
		leftArm.elbowRotate(rotation);
	}

	void rightElbowRotate(float rotation) {
		rightArm.elbowRotate(rotation);
	}

	void lowerLeftArmRotate(float rotation) {
		leftArm.lowerArmRotate(rotation);
	}

	void lowerRightArmRotate(float rotation) {
		rightArm.lowerArmRotate(rotation);
	}

	void headRotate(float pitch, float yaw) {
		head.rotate(pitch, yaw);
	}

	void bodyRotate(float rotation) {
		body.rotate(rotation);
	}

	void draw(mat4 model, mat4 view, mat4 projection);
	Person();
	Person(GenericMesh* buildingBlockMesh, vec3 location, vec3 scale);
private:
	GenericMesh* mesh;
	Arm leftArm;
	Arm rightArm;
	Head head;
	Body body;
};