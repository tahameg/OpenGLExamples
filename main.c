#include <GLFW/glfw3.h>
#include <stdio.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "openGLExample.h"
#include "examples.h"

static GLFWwindow* window;
static double lastTime = 0.0;
static OpenGLExample* currentExample;

static
int oninit()
{
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 0;
    }

    // Set GLFW hints for OpenGL ES 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
    if(!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }
    
    glfwMakeContextCurrent(window);

    currentExample = &ActiveTextureExample;
    currentExample->window = window;
    currentExample->oninit();

    return 1;
}

static 
void ontick(double deltaTime)
{
    if(!currentExample)
    {
        return;
    }
    currentExample->ontick(deltaTime);
}

static
void onexit()
{
    if(currentExample)
    {
        currentExample->onexit();
    }
    currentExample = NULL;
    glfwDestroyWindow(window);
    glfwTerminate();
}

double getDeltaTime()
{
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}

int main()
{
    if(!oninit())
    {
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        ontick(getDeltaTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    onexit();
    return 0;
}