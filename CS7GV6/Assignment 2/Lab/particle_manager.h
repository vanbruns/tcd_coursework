#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "meshes.h"
#include "particle.h"
#include "force_field.h"

#include <list>

class Particle;
class ParticlePortal;
class ParticleGun;
class ForceField;

class ParticleManager {
public:
	void add(ParticlePortal* portal, ParticleGun* gun);
	void clear();
	void remove(Particle& particle);
	void advance(list<ForceField>* forceFields);
	void draw(mat4 view, mat4 projection);

	ParticleManager(GenericMesh* shape, vec3 scale, int cycleWait = 20, int meanNewParticles = 2, int varNewParticles = 1, int meanParticleLifetime = 1500, int varParticleLifetime = 500) {
		mesh = shape;
		vScale = scale;
		iCurrentCycle = 0;
		iCycleWait = cycleWait;
		iMeanParts = meanNewParticles;
		iVarParts = varNewParticles;
		iMeanLifetime = meanParticleLifetime;
		iVarLifetime = varParticleLifetime;
	}
private:
	GenericMesh* mesh;
	vec3 vScale;
	list<Particle> particles;
	list<Particle> markedForRemoval;
	int iCurrentCycle;
	int iCycleWait;
	int iMeanParts;
	int iVarParts;
	int iMeanLifetime;
	int iVarLifetime;
};

#endif //PARTICLE_MANAGER_H