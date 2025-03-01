#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

struct Spring{

    int indexA; // index if 1st sprint attachment
    int indexB; // index if 1st sprint attachment

    // TODO dont need currLen ? 
    double currLen;
    double restLen;

    // spring constants 
    
    // static spring constant
    double ks;
    // dynamic spring constat
    double kd;

    glm::vec3 ForceAtoB;

};

struct Particle{
    // arrays of all the particle info 
    glm::vec3 vel;
    double mass;
    glm::vec3 pos;
    glm::vec3 prevPos;

    // origonal position 
    glm::vec3 pos0;

    bool fixed; // if the particel has fixed pos
    bool groundCol; // ground collision flag
};

class ParticleSystem
{
public:

    int numParticles;
    int numSprings;
    Particle* particles;
    // use std vector for memory managment as we want to be able to add springs dynamicly 
    std::vector<Spring> springs;
    std::vector<Spring> groundSprings;
    // defult constructor
    ParticleSystem(){}
    // give constructor mesh data and spring list 
    ParticleSystem(float* verts, int sizeVerts, double mass);

    ~ParticleSystem(){ delete[] particles; }

    void setSpring(int indexA, int indexB, double restLen, double kd, double ks){
        Spring spring;
        spring.indexA = indexA;
        spring.indexB = indexB;
        spring.restLen = restLen;
        spring.currLen = restLen;
        spring.ks = ks;
        spring.kd = kd;
        springs.push_back(spring);
    }


};

    ParticleSystem::ParticleSystem(float* verts, int sizeVerts, double mass)
    {
        int numVerts = sizeVerts / 3;


        particles = new Particle[numVerts];
        memset(particles, 0, numVerts * sizeof(Particle)); // Ensure no garbage values

        // constuct array of particles  
        for(int i = 0; i < numVerts; i++){
            // use particles[i] directly to stop the varible a from getting dealocated on the stack
            // get each pair of 3 from verts and convert to a particle struct
            particles[i].pos = glm::vec3(verts[i*3], verts[(i*3)+1], verts[(i*3)+2]);
            particles[i].prevPos = particles[i].pos;
            particles[i].pos0 = glm::vec3(verts[i*3], verts[(i*3)+1], verts[(i*3)+2]);
            particles[i].vel = glm::vec3(0.0,0.0,0.0);
            particles[i].fixed = false;
            particles[i].groundCol = false;
            particles[i].mass = mass;

        }
        // TODO particles might still be geting copied somewhere as the address changes (test below and in main)
        //printf("particles allocated at: %p\n", particles);

        numParticles = numVerts;
    }

    


#endif