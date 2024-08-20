// done with lean opengl tutorials 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

// func declare
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// vertex shader - TODO: store in other file?

const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"// the position variable has attribute position 0
    "layout (location = 1) in vec3 aColor;\n"// the color variable has attribute position 1
    "out vec3 ourColour;\n" // output color to frag shader
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n" 
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";

int main(){

    // opengl window settings 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    // create opengl window context (state machine)
    GLFWwindow* window = glfwCreateWindow(800, 600, "ClothSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // initilize glad (load all opengl function pointers)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // RENDER SETUP

    // vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // give shader source code as a string (could pipe from other file)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // now check if shader compilation was succsesfull - TODO check for frag shader too

    int successs;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successs);

    if(!successs){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout <<"ERROR-SHADER-VERTEX-COMPILATION-FAILD\n" << infoLog << std::endl;
    }

    // shader program (link compiled shaders)

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //TODO check if linking was succsefull 


    // delete shader object wich we dont need anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // start using shader program (any call after this will use it)
    // glUseProgram(shaderProgram);

    // element buffer object (ebo)
    // store only uniqe vertices then store what order to render them in
    float vertices[] = {
        // pos              // color
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,// bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    // vertex buffer object
    // vertex array object (rendering config object)
    unsigned int VBO, VAO, EBO;
    // generates buffer object for vertex buffer object
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1,&VAO);

    //1. binds vertex array object, any subsecent calls of relevent func will be stored inside (next non rel func brakes this?)
    glBindVertexArray(VAO);
    // 2. copy vert array in buffer for opengl
    // binds buffer object to the opengl (vertex) array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copys user data to the gl array buffer (in graphics card)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy index array in a element buffer for opengl
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. set vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbind the VBO and VAO so we dont accedently modify them later
    // ---------------------------------------------------------------
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    // ---------------------------------------------------------------

    // set global rendering settings
    // wireframe 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // TODO - delta time
    // RENDER LOOP
    //TODO RENDERING CLASSES
    while(!glfwWindowShouldClose(window))
    {
        // every frame process input 
        processInput(window);


        // background (render first)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // camera 

        
        // rendering commands 
        // 5. draw the object using the VAO object 
        // use the shader program -- may have extra function calls 
        glUseProgram(shaderProgram);
        // use VAO to draw
        glBindVertexArray(VAO);
        // using draw elements as were now using element buffers 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // check events & swap render buffers (display new image)
        glfwSwapBuffers(window);
        glfwPollEvents();    

    }
    // TODO clean opengl rendering recources

    // clean glfw allocations   
    glfwTerminate();
    return 0;
}

// user input function
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// whenver the window size is changed it is updated imeditly (neccisary as opengl uses this for perspective matrices)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}