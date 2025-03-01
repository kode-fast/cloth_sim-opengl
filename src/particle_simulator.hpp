#include <glm/glm.hpp>
#include <glad/glad.h>

#include "particle_system.hpp"

#ifndef PARTICLE_SIMULATOR_HPP
#define PARTICLE_SIMULATOR_HPP
// ParticalSimulator will be to class that runs the whole simulation
class ParticleSimulator
{
public:
	// variables 

	// use a REFERANCE to particle system
	ParticleSystem* system;
    // ground penelty forces 
	double Gks;
	double Gkd;
    // gravity
	double g;
	// global drag coefficient
	double kdrag;

    // assume ground position is (0,0,0) 
	// ground direction vector (points strait up) ( y axis )
	glm::vec3 ground;
	glm::vec3 grav; // -gravity direction

	double Mass;
	glm::vec3 Force;
	int intigrateType;
    // ----------COUNTERS----------
	// number of particles in simulation - get from system ? 
	//int numParticles;
	//int numSprings;
	int numGroundSprings;
	double timeStep;

	// -----LIST / CONTAINERS------
	// TODO where to put spring lists ?
	// array of springs
	// std::vector<Spring*> springs;
	std::vector<Spring*> Groundsprings;


    // functions
	// defult constructor (empty simulation) 
	// this syntax call a defult constructor of particle system varible 
    ParticleSimulator() : system() { Gks = 1; Gkd = 1; g = -9.8; kdrag=1; ground = glm::vec3(0, 1, 0); grav = glm::vec3(0, 1, 0); }
	// pass a const refrence to system becouse its a class 
	// set up partical system with the mesh data provided 

	// pass by REFERANCE and use constructor to assign system to p
	ParticleSimulator(ParticleSystem* p)
	{
		system = p;
		// ground penelty forces 
		Gks = 1;
		Gkd = 1;
		// gravity
		g = -9.8;
		// global drag coefficient
		kdrag =1;
		// number of particles in simulation
		// numParticles = 0;
		// number of springs
		// numSprings = 0;
		// ground vector points sraight up (y is up)
		// N normal vector for ground
		ground = glm::vec3(0, 1, 0);
		// grav vector points straight down (our grav acceleration includs the negative to make this point down)
		grav = glm::vec3(0, 1, 0);

	}

    ~ParticleSimulator();


    int step(double time);
    int update(float* verts, unsigned int VBO);

	void integrate(glm::vec3 pos, glm::vec3 vel, int index);
	void euler(glm::vec3 pos, glm::vec3 vel);
	void symplectic(glm::vec3 pos, glm::vec3 vel);
	void verlet(glm::vec3 pos, glm::vec3 vel, int index);


	void springsForce();
	void groundForcePenelty();
	bool groundCollision(glm::vec3 pos);


};


ParticleSimulator::~ParticleSimulator()
{

}	
/*
// steps the partical simulation 
int ParticleSimulator::step(double time){

	timeStep = time;
	for(int i = 0; i < system->numParticles; i++){

		system->particles[i].pos -= grav*((float)time)*0.1f;

	}

    return 0;
}
*/

// TODO make springs and groundsprings list part of simulator class not system ? 
int ParticleSimulator::step(double time)
{

	// time is sim time

	bool col;
	glm::vec3 gravForce;
	glm::vec3 dragForce;

	glm::vec3 tmp;

	glm::vec3 vel;
	glm::vec3 pos;

	// step 1 CALCULATE FORCE 
	// step 2 find new position and velocity
	// each step calculate spring forces
	
	springsForce();
	groundForcePenelty();

	for (int i = 0; i < system->numParticles; i++) {

		if (system->particles[i].fixed) {
			continue;
		}


		pos = system->particles[i].pos;
		vel = system->particles[i].vel;
		// set mass 
		Mass = system->particles[i].mass;
		// zero the force vector 
		Force = glm::vec3(0.0,0.0,0.0);

		// F = -kd*vel + mass*gravity + External force (springs) / ground ect
		// find all springs with particle i and add spring force to force vector;
		for (int j = 0; j < system->numSprings; j++) {

			if (system->springs[j].indexA == i) {
				// positive
				tmp = system->springs[j].ForceAtoB;
				Force = Force + tmp;

			}

			else if(system->springs[j].indexB == i) {
				// negative
				tmp = system->springs[j].ForceAtoB;
				tmp = tmp * -1.0f;
				Force = Force + tmp;

			}

		}

		// tarible way to do this for optimization.
		for (int k = 0; k < numGroundSprings; k++) {

			if (system->groundSprings[k].indexA == i) {
				// positive

				if (system->particles[i].groundCol) {
					// get force vector
					tmp = system->groundSprings[k].ForceAtoB;
					// add it to Force vector
					Force = Force + tmp;
				}
				else {
					// TODO make spring erase method 
					Groundsprings.erase(Groundsprings.begin() + k);
				}

			}

		}
		
		// add gravety and drag to force vector 

		// drag
		tmp = vel * (float)-kdrag;
		Force = tmp + Force;
		// gravity
		tmp = grav * (float)(g * Mass);
		Force = tmp + Force;

		integrate(pos, vel, i);

		pos = system->particles[i].pos;
		vel = system->particles[i].vel;

		// COLLISION DETECTION AND PENELTY SPRING SETTING
		col = groundCollision(pos);

		if (col && !system->particles[i].groundCol) {

			system->particles[i].groundCol = true;
			Spring* s = new Spring();
			
			// setSpring(int indexA, int indexB, double ks, double kd, double restLength)
			// s.setSpring(i, -1, Gks, Gkd, 0);

			s->indexA = i;
			s->indexB = -1;
			s->kd = Gkd;
			s->ks = Gks;
			s->restLen = 0;

			Groundsprings.push_back(s);

		}
		else if (system->particles[i].groundCol) {
			system->particles[i].groundCol = false;
		}

	}
	
	return 0;

}

// updates the rendered vertices positions from partical simulator step
int ParticleSimulator::update(float* verts, unsigned int VBO){

	// convert system back to a 1d array of positions
	int numVerts = system->numParticles;
	
	// TODO dont need this partArray variable
	// Particle* partArray = system->particles;
	float* newVert = new float[3*numVerts];

	// TODO USE MEMORY POINTER MATH TO DO THIS IN CONSTANT TIME 
	for(int i = 0; i < numVerts; i++){
		printf("(%f, %f, %f)\n", system->particles[i].pos[0], system->particles[i].pos[1], system->particles[i].pos[2]);
		newVert[i*3] = system->particles[i].pos[0];
		newVert[(i*3)+1] = system->particles[i].pos[1];
		newVert[(i*3)+2] = system->particles[i].pos[2];
	}
	printf("-----------------------------------------");

    // Map the buffer for writing
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // WAS USING SIZEOF() WICH WAS MAPPING THE MEMORY WRONG 
    // vert is a pointer so size of was giving just the size of the pointer varible 
    // IMPORTANT: sizeof only gives the size of array memory if its used in the same function as the array was declared 
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, numVerts * 3 * sizeof(float), GL_MAP_WRITE_BIT);

    if (ptr) {
        memcpy(ptr, newVert, numVerts * 3 * sizeof(float)); // Corrected memory copy
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

	// delete allocated memory and unbind buffer
	delete[] newVert;
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

void ParticleSimulator::integrate(glm::vec3 pos, glm::vec3 vel, int index) {

	// integration time step is independent of simulation time step?
	
	if (intigrateType == 0){
		euler(pos, vel);
	}
	else if (intigrateType == 1) {
		symplectic(pos, vel);
	}
	// varlet
	else {
		// if its the first iteration use euler - def better way of doing this 
		glm::vec3 tmp;
		tmp = system->particles[index].pos0;
		// prev position was never updated after euler 

		// TODO add tolerance to equal ? (p1, p2, 0.00001f)
		if ( glm::all(glm::equal(system->particles[index].prevPos, tmp)) ){
			//animTcl::OutputMessage("euler");
			euler(pos, vel);

			// TODO make elment wise vector assignment function ?
			system->particles[index].prevPos = pos;
		}
		else {
			verlet(pos, vel, index);
		}
	}

}


void ParticleSimulator::euler(glm::vec3 pos, glm::vec3 vel) {


	glm::vec3 tmp;

	// position
	tmp = vel * (float)timeStep;
	pos = pos + tmp;

	// pos = pos + vel * timestep
	
	// velocity 
	tmp = Force * (float)(1.0/Mass);
	tmp = tmp * (float)timeStep;
	vel = vel + tmp;

	// vel = vel + timestep * Force / Mass


}

void ParticleSimulator::symplectic(glm::vec3 pos, glm::vec3 vel) {

	glm::vec3 tmp;

	// velocity 
	tmp = Force * (float)(1.0 / Mass);
	tmp = tmp * (float)timeStep;
	vel = vel + tmp;
	// vel = vel + timestep * Force / Mass
	
	// position
	tmp = vel * (float)timeStep;
	// TODO set with system->particles[index].pos ? 
	pos = pos + tmp;

	// pos = pos + new-vel * timestep

}

void ParticleSimulator::verlet(glm::vec3 pos, glm::vec3 vel, int index) {
	// acceleration = prev vel - vel * timestep
	

	// check if its the first iteration - if prevpos = pos0;
	// then use euler insted 

	// TODO dont need line bollow right?
	// prevPos[index];
	// dont need prevVel prevVel[index] ;

	// F =ma,  a = F/m
	glm::vec3 tmp;
	glm::vec3 accel;
	glm::vec3 newPos;
	// position
	// = 2 * pos - prev pos + accelration->(F/m) * timestep ^2

	// 2 * pos
	tmp = pos * 2.0f;

	// accelration->(F/m) * timestep ^2
	accel = Force * (float)(1.0/Mass);
	accel = accel * (float)pow(timeStep, 2);

	// 2 * pos - prev pos
	tmp = tmp - system->particles[index].prevPos;

	// 2 * pos - prev pos + accelration->(F/m) * timestep ^2
	newPos = tmp + accel;

	// velocity 
	// = new pos - prev pos / 2* timestep

	vel = newPos - system->particles[index].prevPos;
	vel = vel * (float)(1.0 / 2 * timeStep);

	// set pos and prev pos 
	system->particles[index].prevPos = pos;
	system->particles[index].pos = newPos;
	
}

// calculating spring force sepretly at the start 
// to avoid paricles moving while calculating 
void ParticleSimulator::springsForce() {

	glm::vec3 force;
	glm::vec3 SpringForce;
	glm::vec3 DampForce;
	
	// position
	glm::vec3 xi;
	glm::vec3 xj;
	// velocity
	glm::vec3 vi;
	glm::vec3 vj;

	glm::vec3 forceDirc;
	glm::vec3 tmp;
	glm::vec3 disVec;

	// GLM use float persision so must cast to float 
	// TODO change everything to float
	// 		or use doubles?
	float length;
	float lenWeight;


	// do we need to update? no do it in set springs
	//springs = particleObject->getSprings();

	for (int i = 0; i < system->numSprings; i++) {
		// SpringForce = -spring.ks() * ( spring.length() - length(xi - xj) ) *  ( xi - xj / length (xi - xj) )		
	

		// ------- SPRING FORCE---------
		// get positions of xi and xj
		xi = system->particles[system->springs[i].indexA].pos;
		xj = system->particles[system->springs[i].indexB].pos;

		// more stable version
		// (rest lenght * (xi - xj) / length )- ks
		// calculate force direction = ( xi - xj / abs( xi - xj ))
		// = xi - xj
		disVec = xi - xj;
		// = length (xi - xj)
		length = glm::length(disVec);
		// forceDirc = ( xi - xj )/ length(xi,xj)

		forceDirc =  disVec * (1.0f / length);

		tmp = forceDirc * (float)system->springs[i].restLen;
		tmp = tmp - disVec;

		SpringForce = tmp * (float)system->springs[i].ks;

		// -------- DAMPER FORCE ---------
		// DampForce = -spring.kd() *  ( ( vi - vj ) dotproduct ( xi - xj / length xi - xj ) ) ( xi - xj / length xi - xj)

		vi = system->particles[system->springs[i].indexA].vel;
		vj = system->particles[system->springs[i].indexB].vel;

		// vector between velocity vectors 
		tmp = vi - vj ;

		lenWeight = glm::dot(tmp, forceDirc);


		DampForce = forceDirc * (lenWeight * ( (float)-system->springs[i].kd) );

		// -------- ADD FORCES -----------------

		force = SpringForce + DampForce;

		// set spring force
		system->springs[i].ForceAtoB = force;

	}

}


void ParticleSimulator::groundForcePenelty() {

	glm::vec3 forceNorm;
	glm::vec3 SpringForce;
	glm::vec3 DampForce;

	// arbatrary point on the ground plain
	glm::vec3 point = { 0,0,0 };

	// position
	glm::vec3 pos;
	// velocity
	glm::vec3 vel;

	glm::vec3 forceDirc;
	glm::vec3 tmp;

	// GLM uses float precission
	float length;
	float lenWeight;
	
	for (int i = 0; i < numGroundSprings; i++) {

		// ------- GROUND SPRING FORCE---------
		// get positions of xi 
		pos = system->particles[system->groundSprings[i].indexA].pos;

		// this is equal to pos
		//VecSubtract(forceDirc, pos, point);
		
		// -ks * ( pos - point on plane ) dotprod ground normal
		SpringForce = ground * (float)(- system->groundSprings[i].ks * glm::dot(pos, ground));

		// ------- GROUND Dampen FORCE---------
		vel = system->particles[system->groundSprings[i].indexA].vel;

		// -kd * (vel dot ground norm) * ground norm
		DampForce = ground * (float)( - system->groundSprings[i].kd * glm::dot(vel, ground) );

		// add both terms
		forceNorm = SpringForce * DampForce;

		// TODO use referance for this assignment the varible forceNorm dosnt get out of scope 
		system->groundSprings[i].ForceAtoB = forceNorm;

	}



}

bool ParticleSimulator::groundCollision(glm::vec3 pos) {

	glm::vec3 v;
	glm::vec3 zero = glm::vec3(0, 0, 0);
	// v is vector from ground orgine to particle position 
	// NOTE we can just set position as vector v as pos - 0 = pos
	// VecSubtract(v, pos, zero);
	
	double dot = glm::dot(pos, ground);

	// if dot product is negative pos is below the ground 
	if (dot < 0) {
		return true;
	}
	else {
		return false;
	}

}


#endif