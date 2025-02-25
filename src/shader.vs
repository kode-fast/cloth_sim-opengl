//VERTEX SHADER

#version 460 core

layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourColor; // output color to frag shader
  
uniform float time; // Animation time
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// uniform buffer variable for bakeing positions?  

// NOTE we can make a uniform variable of all the curent positions of particles/verts 
// could solve some problems for shader based cloth sim? 
void main()
{

    //gl_Position = projection * view * model * vec4(aPos, 1.0f);
    
    // wave effect 
    vec3 transformedPos = aPos;
    transformedPos.y += sin(aPos.x + time); // Wave effect along Y-axis
    gl_Position = projection * view * model * vec4(transformedPos, 1.0);

    

    ourColor = aColor;
}
