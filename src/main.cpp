// done with lean opengl tutorials from learnopengl.com
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

// func declare
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// settings
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  (float)WIDTH / 2.0;
float lastY =  (float)HEIGHT / 2.0;

float deltaTime = 0.0f; 
float lastFrame = 0.0f;

// set camera positions

// setting for camera distace ? = 3.0
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

// TODO make camera front consistent thruoght the program (look at input function)
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

int main(){

    // opengl window settings 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    // create opengl window context (state machine)
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ClothSim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // register window with varius functions
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell window to capture mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initilize glad (load all opengl function pointers)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // RENDER SETUP
    
    //SHADERS 
    // crate shader class object - compile 
    Shader ourShader("src/shader.vs", "src/shader.fs");


    // element buffer object (ebo)
    // store only uniqe vertices then store what order to render them in
    // DONT NEED TO USE RN
    /*
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

    */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // TODO make mesh class
    
    // vertex buffer object
    // vertex array object (rendering config object)

    unsigned int VBO, VAO; //, EBO;
    // generates buffer object for vertex buffer object
    glGenBuffers(1,&VBO);
    //glGenBuffers(1, &EBO);
    glGenVertexArrays(1,&VAO);

    //1. binds vertex array object, any subsecent calls of relevent func will be stored inside (next non rel func brakes this?)
    glBindVertexArray(VAO);
    // 2. copy vert array in buffer for opengl
    // binds buffer object to the opengl (vertex) array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copys user data to the gl array buffer (in graphics card)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy index array in a element buffer for opengl
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. set vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // TODO, USE NORMALIZED POSITION AS COLOUR ATRABUTES
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
    // enable z-buffer
    glEnable(GL_DEPTH_TEST);
    

    
    // TODO - make animation class
    // TODO - delta time
    // RENDER LOOP
    //TODO RENDERING CLASSES
    while(!glfwWindowShouldClose(window))
    {
        // CALC DELTA TIME
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // every frame process input 
        processInput(window);
        // background (render first)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // clear z-buffer and colur buffer for next frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();
        //ourShader.setFloat("someUniformVariable", 1.0f);

        // create transformations 
        // TODO - use a model class to make this simpler 
        
        // set matrices to I matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        double time = glfwGetTime();
        // model matrix - translates from object -> world (aka transforms objects around the sceane)
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        model = glm::translate(model, glm::vec3(sin(time)+0.2, cos(time)+0.5, 0.0 ));  
        model = glm::rotate(model, (float)time, glm::vec3(0.0, 0.0, 1.0));

        // view matrix - translates from world -> view (puts camera at 0,0,0 on the z axis)
        // in this case were moving the camera back 3 units (or the sceane forward)
        //view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));

        // use glm::look at to create a camera look at matrix 
        /* ex:
        view = glm::lookAt( glm::vec3(0.0f, 0.0f, 3.0f), 
  		                    glm::vec3(0.0f, 0.0f, 0.0f), 
  		                    glm::vec3(0.0f, 1.0f, 0.0f));

        // another ex of spining camera
        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        */
        

        // lookAt func takes camera position, target position (where we want the camera to look at), world up vector
        // view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // view = glm::lookAt(cameraPos, cameraPos + ( 3.0f * cameraFront ) , cameraUp);
        view = glm::lookAt(cameraPos, cameraTarget , cameraUp);

        // projection matrix - translates from view -> clip space / screen space
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // now send each matrix to our perspective shader uning uniform variable

        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        modelLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));

        modelLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection));


        // 5. draw the object using the VAO object 
        // use VAO to draw
        glBindVertexArray(VAO);

        // BACK TO DRAWING ARRAYS 
        // using draw elements as were now using element buffers 
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // call draw arrays 10 time, but send diffrent model matrix to shader each time
        
        // model transfomations need to be in the for loop to apply to each cube
        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; 
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
              
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

    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
        cameraTarget -= cameraSpeed * cameraFront;
        //std::cout << cameraTarget.x , cameraTarget.y, cameraTarget.z;

    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos += cameraSpeed * cameraFront;
        cameraTarget += cameraSpeed * cameraFront;
        //std::cout << cameraTarget.x , cameraTarget.y, cameraTarget.z;

    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cameraTarget += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //std::cout << cameraTarget.x , cameraTarget.y, cameraTarget.z;

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        cameraTarget -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //std::cout << "--------------" , cameraTarget.x , cameraTarget.y, cameraTarget.z ,"---------";

    }


    

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.9f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

 // dont need pich lock for rotation camera
// TODO fix camera fliping when going over 90 degrees 
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    //glm::vec3 direction;
    /*fly style camera
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    */

    glm::vec3 newPos;
    // orbit style camera 

    // pitch - rotate around x and z (up down)
    // yaw - rotate around y (left right)

    newPos.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newPos.y = sin(glm::radians(pitch));
    newPos.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    // multiply by length to target / rotation radius 
    // TODO - find better way to do this (w/out sqrt)
    cameraPos = glm::length(cameraPos) * glm::normalize((cameraPos - newPos));
    cameraFront = glm::normalize(cameraPos - cameraTarget);
}




// whenver the window size is changed it is updated imeditly (neccisary as opengl uses this for perspective matrices)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}