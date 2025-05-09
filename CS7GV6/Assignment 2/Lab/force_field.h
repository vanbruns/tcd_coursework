#ifndef FORCE_FIELD_H
#define FORCE_FIELD_H

#include "particle.h"

class ForceField {
public:
	bool isInField(vec3 position) {
		return position.v[0] > vSmallCorner.v[0] && position.v[0] < vLargeCorner.v[0] &&
				position.v[1] > vSmallCorner.v[1] && position.v[1] < vLargeCorner.v[1] &&
				position.v[2] > vSmallCorner.v[2] && position.v[2] < vLargeCorner.v[2];
	}

	vec3 affectDirection(vec3 direction, vec3 position) {
		if (fInward != 0.0f) {
			return normalise(direction - (position - vCenter) * fInward);
		}
		else {
			return normalise(direction - vDirection);
		}
	}

	ForceField(vec3 direction, vec3 smallCorner, vec3 largeCorner, float inwardFactor = 0.0f) {
		vDirection = direction;
		vSmallCorner = smallCorner;
		vLargeCorner = largeCorner;
		vCenter = (vLargeCorner + vSmallCorner) / 2.0f;
		fInward = inwardFactor;
	}
private:
	vec3 vDirection;
	vec3 vCenter;
	vec3 vSmallCorner;
	vec3 vLargeCorner;
	float fInward;
};

#endif //FORCE_FIELD_H