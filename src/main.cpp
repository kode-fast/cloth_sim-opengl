// done with learn opengl tutorials from learnopengl.com
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

#include "particle_system.hpp"
#include "particle_simulator.hpp"

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

double deltaTime = 0.0; 
double lastFrame = 0.0;

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
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // generate sheet 
    int width = 100;
    int height = 100;
    float increment = 1;

    

    for(int j = 0; j<height; j++){
        for(int i=0; i<width; i++){
            
        }

    }

    // TODO can seperate color into a diffrent array for easy modification of verts 
    // 10 Cubes - testing  
    float vertices[] = {
    // Positions         
    -0.5f, -0.5f, -0.5f, // 0 - Back Bottom Left
     0.5f, -0.5f, -0.5f, // 1 - Back Bottom Right
     0.5f,  0.5f, -0.5f, // 2 - Back Top Right
    -0.5f,  0.5f, -0.5f, // 3 - Back Top Left
    -0.5f, -0.5f,  0.5f, // 4 - Front Bottom Left
     0.5f, -0.5f,  0.5f, // 5 - Front Bottom Right
     0.5f,  0.5f,  0.5f, // 6 - Front Top Right
    -0.5f,  0.5f,  0.5f // 7 - Front Top Left
    };

    // 0 here to?
    printf("%f", vertices[2]);
    // TODO can set up particle system at this point after we've set up mesh 
    // can use sizeof() here becouse vertices is declared in this file 
    ParticleSystem particles(vertices, sizeof(vertices)/sizeof(float), 1.0);
    // set up springs 

    // declare particle simulator 
    // give POINTER to particles so it doesnet gets passed by referance
    ParticleSimulator simulation(&particles);
    
    printf("particles address in main: %p\n", particles);

    // particles array decaying 
    printf("main test: %f\n", particles.particles[0].pos[0]);

    // mass also decaying ??
    printf("main test particles mass: %f\n", particles.particles[0].mass);


    // DEBUG RETURN remove
    //return  0;

    float color[]{
    // Color 
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f 
    };


    unsigned int indices[] = {  
    // Back face
    0, 1, 2, 2, 3, 0, 
    // Front face
    4, 5, 6, 6, 7, 4, 
    // Left face
    0, 3, 7, 7, 4, 0, 
    // Right face
    1, 5, 6, 6, 2, 1, 
    // Bottom face
    0, 1, 5, 5, 4, 0, 
    // Top face
    3, 2, 6, 6, 7, 3
    };  

    // TODO make mesh / object class
    // -> this is to wrap the creation and rendering settings of an object into a subroutine that can be easly called

    // TODO make spring cube - for colision well have to be able to spawn new springs 
    // -> it would be usefull (for debug) to connect springs with edges, meaning spring mass class will have to intake the vertex array, with the same order of edge as glDrawArray(TRIANGLE)
    // -> could also use EBO(element buffer object to define edges more easily) actualy i think we have to, as the edges have to be conected to the same vertices 

    // vertex buffer object
    // vertex array object (rendering config object)
    unsigned int VBO, VAO, EBO;// 
    // generates buffer object for vertex buffer object
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1,&VAO);

    //1. binds vertex array object, any subsecent calls of relevent func will be stored inside (next non rel func brakes this?)
    glBindVertexArray(VAO);
    // 2. copy vert array in buffer for opengl
    // binds buffer object to the opengl (vertex) array buffer
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    // copys user data to the gl array buffer (in graphics card)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. copy index array in a element buffer for opengl
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4. set vertex attributes pointers
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // TODO, USE NORMALIZED POSITION AS COLOUR ATRABUTES
    // color attribute 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);


    /*
    // WORKS ! :)
    // for it to work in loop we have to not unbinde the VBO after the setup above
    // vertex modification test -------------------------------

    // TODO - use glMapBufferRange to get mesh in buffer 
    float newVertices[] = {
    // Positions          // Color 
    0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,// 2 - Back Top Right
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// 3 - Back Top Left
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,// 4 - Front Bottom Left
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,// 5 - Front Bottom Right
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// 0 - Back Bottom Left
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,// 1 - Back Bottom Right
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// 6 - Front Top Right
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f // 7 - Front Top Left
    };

    // get pointer to gpu memory - remeber we dont use variable for this the opengl constant will give us the buffer memory
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(newVertices), GL_MAP_WRITE_BIT);
    // memcpy copys the block of memorey from the array to the 
    memcpy(ptr, newVertices, sizeof(newVertices));

    // unmap the buffer (HAVE TO DO becuse opengl locks the buffer when its mapped)

    glUnmapBuffer(GL_ARRAY_BUFFER);
    */

    // -------------------------- -------------------------------
    

    // unbind the VBO and VAO so we dont accedently modify them later
    // ---------------------------------------------------------------
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.

    glBindVertexArray(0); 
    // ---------------------------------------------------------------

    // TODO - drawMesh function or part of a class to simplify loop and alow multipul passes on the same mesh (ie solid and wire frame)

    // set global rendering settings
    // wireframe 
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // solid 
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable z-buffer
    glEnable(GL_DEPTH_TEST);
    

    
    // TODO - make animation class ?


    // RENDER LOOP
    //TODO RENDERING CLASSES
    while(!glfwWindowShouldClose(window))
    {
        // CALC DELTA TIME
        double time = glfwGetTime();
        deltaTime = time - lastFrame;
        lastFrame = time;
        // step the simulation
        simulation.step(deltaTime);

        printf("delta time : %f\n", deltaTime);
        // every frame process input 
        processInput(window);
        // background (render first)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // clear z-buffer and colur buffer for next frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // UPDATE WORKS :)
        // update vertex positions


        // activate shader
        ourShader.use();
        //ourShader.setFloat("someUniformVariable", 1.0f);

        // create transformations 
        // TODO - use a model class to make this simpler 
        
        // set matrices to I matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        /*
        // model matrix - translates from object -> world (aka transforms objects around the sceane)
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
        model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
        model = glm::translate(model, glm::vec3(sin(time)+0.2, cos(time)+0.5, 0.0 ));  
        model = glm::rotate(model, (float)time, glm::vec3(0.0, 0.0, 1.0));
        */
        // view matrix - translates from world -> view (puts camera at 0,0,0 on the z axis)
        // lookAt func takes camera position, target position (where we want the camera to look at), world up vector
        view = glm::lookAt(cameraPos, cameraTarget , cameraUp);

        // projection matrix - translates from view -> clip space / screen space
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // now send each matrix to our perspective shader using uniform variable
        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // get location of delta time var in vs shader
        int timeLoc = glGetUniformLocation(ourShader.ID, "time");
        // set uniform variable
        glUniform1f(timeLoc, time);

        ourShader.setMat4("model", model);

        // 5. draw the object using the VAO object 
        // use VAO memoery to draw
        glBindVertexArray(VAO);

        // RENDER BLOCK
        // update vertices from the simulation
        simulation.update(vertices, VBO);

        // we can change the drawing polygone mode on the fly and draw the model as meny times as we want
        glPointSize(8);
        // using draw elements becouse of EBO
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // glDrawElements <- mode, count - the # of elements to be rendered (ie number 
        // of indexes 
            
        // END RENDER   
        glBindVertexArray(0); 

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

    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
            
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
    

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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
}




// whenver the window size is changed it is updated imeditly (neccisary as opengl uses this for perspective matrices)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}