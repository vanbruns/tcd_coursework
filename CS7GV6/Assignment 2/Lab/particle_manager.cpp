#include "particle_manager.h"

void ParticleManager::add(ParticlePortal* portal, ParticleGun* gun) {
	if (iCurrentCycle < iCycleWait) {
		iCurrentCycle++;

		return;
	}
	else {
		iCurrentCycle = 0;
	}

	int newParts = iMeanParts + rand() % iVarParts;

	for (int i = 0; i < newParts; i++) {
		int lifetime = iMeanLifetime + rand() % iVarLifetime;

		Particle particle = Particle(mesh, portal->createEntryPoint(), gun->createTrajectory(), gun->createSpeed(), vScale, lifetime);

		particle.setManager(this);

		particles.push_back(particle);
	}
}

void ParticleManager::clear() {
	particles.clear();
	markedForRemoval.clear();
}

void ParticleManager::remove(Particle& particle) {
	markedForRemoval.push_back(particle);
}

void ParticleManager::advance(list<ForceField>* forceFields) {
	for (list<Particle>::iterator particle = markedForRemoval.begin(); particle != markedForRemoval.end(); particle++) {
		particles.remove(*particle);
	}

	markedForRemoval.clear();

	for (list<Particle>::iterator particle = particles.begin(); particle != particles.end(); particle++) {
		for (list<ForceField>::iterator field = forceFields->begin(); field != forceFields->end(); field++) {
			particle->affect(*field);
		}

		particle->advance();
	}
}

void ParticleManager::draw(mat4 view, mat4 projection) {
	for (list<Particle>::iterator particle = particles.begin(); particle != particles.end(); particle++) {
		particle->draw(view, projection);
	}
}