#include "particle.h"

void Particle::advance() {
	vPosition = vPosition + vDirection * fSpeed;
	iLifetime--;

	if (iLifetime == 0) {
		pm->remove(*this);
	}
}

void Particle::setManager(ParticleManager* manager) {
	pm = manager;
}