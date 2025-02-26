
#include <glm/glm.hpp>
#include <glad/glad.h>

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

struct Spring{

    int pointA; // index if 1st sprint attachment
    int pointB; // index if 1st sprint attachment
    double currLen;
    double restLen;

};

struct Particle{
    // arrays of all the particle info 
    double* vel;
    double* mass;
    double* pos;
    double* prevPos;

};

class ParticleSystem
{
    int numParticles;
    int numSprings;
    Particle* particles;
    Spring* springs;

};



// TEST UPDATE FUNCTION
int update(float* vert, double deltaTime, unsigned int VBO){
    float* newVert = vert;
    int vertexCount = 8;

    for (int i = 1; i < vertexCount * 3; i += 3) {
        vert[i] -= 9.8f * deltaTime * 0.1;  
    }

    // Map the buffer for writing
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // WAS USING SIZEOF() WICH WAS MAPPING THE MEMORY WRONG 
    // vert is a pointer so size of was giving just the size of the pointer varible 
    // IMPORTANT: sizeof only gives the size of array memory if its used in the same function as the array was declared 
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexCount * 3 * sizeof(float), GL_MAP_WRITE_BIT);

    if (ptr) {
        memcpy(ptr, newVert, vertexCount * 3 * sizeof(float)); // Corrected memory copy
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    return 0;
}

#endif