
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

    Particle* particles;
    Spring* springs;

};





// TEST UPDATE FUNCTION
float* update(float* vert){

    float* newVertices = vert;
    // TODO could reinterpret_cast the 1d array to a 2d array for sim
    
    /*
    // size of vert is 8 
    printf("%d",sizeof(vert));

    // this prints 8 digits not 8 paris of 3 - need to multiply by size of data type to get total size of the array 
    for(int i = 0; i < sizeof(vert)*sizeof(double);i++){
        printf("%f\n", vert[i]);

    }
    */
   
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