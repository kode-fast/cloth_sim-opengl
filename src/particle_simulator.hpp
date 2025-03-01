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


    // ----------COUNTERS----------
	// number of particles in simulation - get from system ? 
	//int numParticles;
	//int numSprings;


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


};


ParticleSimulator::~ParticleSimulator()
{

}	

// steps the partical simulation 
int ParticleSimulator::step(double time){

	printf("------------------ step function ------------------\n");
	printf("(%f, %f, %f)\n", system->particles[0].pos[0], system->particles[0].pos[1], system->particles[0].pos[2]);

	for(int i = 0; i < system->numParticles; i++){
		printf("(%f, %f, %f)\n", system->particles[i].pos[0], system->particles[i].pos[1], system->particles[i].pos[2]);
		printf("edit\n");
		system->particles[i].pos[1] -= 9.8f*((float)time)*0.1f;
		printf("(%f, %f, %f)\n", system->particles[i].pos[0], system->particles[i].pos[1], system->particles[i].pos[2]);

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


#endif