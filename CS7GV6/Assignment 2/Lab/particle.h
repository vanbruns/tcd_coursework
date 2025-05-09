#ifndef PARTICLE_H
#define PARTICLE_H

#include "meshes.h"
#include "particle_manager.h"
#include "force_field.h"

#include <list>

using namespace std;

class ParticleManager;

// Defines how a particle enters the system
class ParticlePortal {
public:
	vec3 createEntryPoint() {
		int spaces = iStepCount - 1;
		float x = vSmallCorner.v[0] + (rand() % iStepCount) * (vLargeCorner.v[0] - vSmallCorner.v[0]) / spaces;
		float y = vSmallCorner.v[1] + (rand() % iStepCount) * (vLargeCorner.v[1] - vSmallCorner.v[1]) / spaces;
		float z = vSmallCorner.v[2] + (rand() % iStepCount) * (vLargeCorner.v[2] - vSmallCorner.v[2]) / spaces;

		return vec3(x, y, z);
	}

	ParticlePortal(int stepCount, vec3 smallCorner, vec3 largeCorner) {
		iStepCount = stepCount;
		vSmallCorner = smallCorner;
		vLargeCorner = largeCorner;
	}
private:
	int iStepCount;
	vec3 vSmallCorner;
	vec3 vLargeCorner;
};

// Defines how a particle is directed into the system
class ParticleGun {
public:
	vec3 createTrajectory() {
		mat4 rotation = identity_mat4();

		rotation = rotate_x_deg(rotation, (rand() % iStepCount) * vRotation.v[0] / (iStepCount - 1));
		rotation = rotate_y_deg(rotation, (rand() % iStepCount) * vRotation.v[1] / (iStepCount - 1));
		rotation = rotate_z_deg(rotation, (rand() % iStepCount) * vRotation.v[2] / (iStepCount - 1));

		return vec3(rotation * vec4(vInitialDirection, 1.0f));
	}

	float createSpeed() {
		return fMeanSpeed + fVarSpeedFactor * (rand() % iSpeedStep);
	}

	ParticleGun(vec3 initialDirection, int stepCount, vec3 rotationRangeSize, float meanSpeed = 0.015, float varSpeedFactor = 0.005, int speedStep = 10) {
		vInitialDirection = initialDirection;
		iStepCount = stepCount;
		vRotation = rotationRangeSize;
		fMeanSpeed = meanSpeed;
		fVarSpeedFactor = varSpeedFactor;
		iSpeedStep = speedStep;
	}
private:
	vec3 vInitialDirection;
	int iStepCount;
	vec3 vRotation;
	float fMeanSpeed;
	float fVarSpeedFactor;
	int iSpeedStep;
};

class Particle {
public:
	// Comparison is required for using in collections.
	friend bool operator==(const Particle& lhs, const Particle& rhs) {
		return vec3Compare(lhs.vPosition, rhs.vPosition) &&
			vec3Compare(lhs.vDirection, rhs.vDirection) &&
			vec3Compare(lhs.vScale, rhs.vScale) &&
			lhs.fSpeed == rhs.fSpeed &&
			lhs.iLifetime == rhs.iLifetime;
	}

	void draw(mat4 view, mat4 projection) {
		mat4 orient = scale(identity_mat4(), vScale);
			
		orient = translate(orient, vPosition);

		mesh->draw(orient, view, projection);
	}

	void affect(ForceField field) {
		if (field.isInField(vPosition)) {
			vDirection = field.affectDirection(vDirection * fSpeed, vPosition);
		}
	}

	void advance();
	void setManager(ParticleManager* manager);

	Particle(GenericMesh* shape, vec3 position, vec3 direction, float speed, vec3 scale, int lifetime) {
		mesh = shape;
		vPosition = position;
		vDirection = direction;
		fSpeed = speed;
		vScale = scale;
		iLifetime = lifetime;
	}
private:
	ParticleManager* pm;
	GenericMesh* mesh;
	vec3 vPosition;
	vec3 vDirection;
	vec3 vScale;
	float fSpeed;
	int iLifetime;

	static bool vec3Compare(const vec3& lhs, const vec3& rhs) {
		return lhs.v[0] == rhs.v[0] &&
			lhs.v[1] == rhs.v[1] &&
			lhs.v[2] == rhs.v[2];
	}
};

#endif //PARTICLE_H