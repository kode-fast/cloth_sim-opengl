#include "particle_system.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>

#ifndef PARTICLE_SIMULATOR_HPP
#define PARTICLE_SIMULATOR_HPP
// ParticalSimulator will be to class that runs the whole simulation
class ParticleSimulator
{
public:

    // functions 
    ParticleSimulator();
	ParticleSimulator(float* vert);
    ~ParticleSimulator();


    int step(double time);
    float* update(float* vert);

    // variables 

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
	// number of particles in simulation
	int numParticles;
	int numSprings;
};

// set up empty simulation
ParticleSimulator::ParticleSimulator() { Gks = 1; Gkd = 1; g = -9.8; kdrag =1; numParticles = 0; numSprings = 0; ground = glm::vec3(0, 1, 0); grav = glm::vec3(0, 1, 0); }	

// set up partical system with the mesh data provided 
ParticleSimulator::ParticleSimulator(float* vert)
{
	// ground penelty forces 
	Gks = 1;
	Gkd = 1;

	// gravity
	g = -9.8;

	// global drag coefficient
	kdrag =1;

	// number of particles in simulation
	numParticles = 0;
	// number of springs
	numSprings = 0;

	// ground vector points sraight up (y is up)
	// N normal vector for ground
	ground = glm::vec3(0, 1, 0);
	// grav vector points straight down (our grav acceleration includs the negative to make this point down)
	grav = glm::vec3(0, 1, 0);

}

ParticleSimulator::~ParticleSimulator()
{

}	

// steps the partical simulation 
int ParticleSimulator::step(double time){


    return 0;

};


// updates the rendered vertices positions from partical simulator step
float* ParticleSimulator::update(float* vert){


    float* newVertices = vert;
    printf("%f", vert[0]);

    vert[0] = vert[0] + 0.5;
    vert[1] = vert[1] + 0.5;

    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(newVertices), GL_MAP_WRITE_BIT);
    // memcpy copys the block of memorey from the array to the 
    memcpy(ptr, newVertices, sizeof(newVertices));

    // unmap the buffer (HAVE TO DO becuse opengl locks the buffer when its mapped)

    glUnmapBuffer(GL_ARRAY_BUFFER);

    return newVertices;
}


#endif