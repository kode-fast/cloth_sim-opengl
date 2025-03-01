#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

struct Spring{

    int indexA; // index if 1st sprint attachment
    int indexB; // index if 1st sprint attachment
    double currLen;
    double restLen;
    // defult constructor for struct
    Spring() : indexA(0.0f), indexB(0.0f), currLen(0.0), restLen(1.0) {} 
};

struct Particle{
    // arrays of all the particle info 
    glm::vec3 vel;
    double mass;
    glm::vec3 pos;
    glm::vec3 prevPos;
    // defult constructor for struct
    Particle() : pos(0.0f), prevPos(0.0f), vel(0.0f), mass(1.0) {} 

};

class ParticleSystem
{
public:

    int numParticles;
    int numSprings;
    Particle* particles;
    // use std vector for memory managment as we want to be able to add springs dynamicly 
    std::vector<Spring> springs;

    // defult constructor
    ParticleSystem(){}
    // give constructor mesh data and spring list 
    ParticleSystem(float* verts, int sizeVerts, double mass);

    ~ParticleSystem(){ delete[] particles; }

    void setSpring(int indexA, int indexB, double restLen){
        Spring spring;
        spring.indexA = indexA;
        spring.indexB = indexB;
        spring.restLen = restLen;
        spring.currLen = restLen;
        springs.push_back(spring);
    }


};

    ParticleSystem::ParticleSystem(float* verts, int sizeVerts, double mass)
    {
        printf("%f", verts[0]);
        int numVerts = sizeVerts / 3;


        particles = new Particle[numVerts];
        memset(particles, 0, numVerts * sizeof(Particle)); // Ensure no garbage values

        // constuct array of particles  
        for(int i = 0; i < numVerts; i++){
            // need to allocate memory for Particle a in heap so it dosnt get deleted in the stack?
            // use particles[i] directly to stop the varible a from getting dealocated on the stack
            // Particle a;
            // get each pair of 3 from verts and convert to a particle struct
            particles[i].pos = glm::vec3(verts[i*3], verts[(i*3)+1], verts[(i*3)+2]);
            particles[i].prevPos = particles[i].pos;
            particles[i].vel = glm::vec3(0.0,0.0,0.0);
            particles[i].mass = mass;

            //printf("(%f, %f, %f)\n", verts[i*3], verts[(i*3)+1], verts[(i*3)+2]);
            //printf("(%f, %f, %f)\n", particles[i].pos[0], particles[i].pos[1], particles[i].pos[2]);

        }
        printf("particles allocated at: %p\n", particles);

        numParticles = numVerts;
    }



#endif