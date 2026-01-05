
#include "glad/glad.h"
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>

#include <iostream>
#include <cmath>
#include <time.h>

#include "shader/shader.h"
#include "model/circle.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float getTime(float _centery, float _radius);
float calculateCenterY(float timeMoment, float timeFinal, float _radius);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600; 

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "gravity-sim", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Circle circle(100, 0, 0.0, 0.1, pow(10, 10));
    Circle circle2(100, 0.9, 0.9, 0.02, pow(10, 4));
    // Circle circle3(100, -0.7, 0.4, 0.1);
    Circle circles[] = {circle, circle2};
    Shader myShader("shader/src/shader.vs", "shader/src/shader.fs");
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myShader.use();

        for(int i = 0; i < sizeof(circles)/sizeof(Circle); i++){

            circles[i].use();

            
            if(sizeof(circles)/sizeof(Circle) == 1)
            {
                circles[0].acelerate(nullptr);
            }

            for(int j = 0; j < sizeof(circles)/sizeof(Circle); j++){
                if(j != i) circles[j].acelerate(&circles[i]);
            }

            circles[i].changePosition();



        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    for(int i = 0; i < sizeof(circles)/sizeof(Circle); i++)
        circles[i].deleteBuffers();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

