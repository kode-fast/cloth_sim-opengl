
// define a spring mass class here 

// we need a class for a spring as we need to beable to spawn springs sepretly 
// and a class for spring mass
// -> a class for spring mas would incude calculations right?
// -> therfor all we actualy need (for simples implemt ) is a struct with the variable of each vector
// -> the calculation will be done in the main file ( as we dont have a rendering or object class yet )
// -> or clac done in shader file 

// try keep it simple could be structs?
// - no not really what i need

#ifndef SPRING_MASS_H
#define SPRING_MASS_H

struct SpringMass
{

    float Mass;
    double currLen;
    double restLen;

};


float* update(float* vert){

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