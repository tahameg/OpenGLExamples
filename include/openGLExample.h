#ifndef OPENGL_EXAMPLE_H
#define OPENGL_EXAMPLE_H 1
struct GLFWWindow;
typedef struct GLFWwindow GLFWwindow;

typedef struct 
{
    const char name[64];
    const char description[256];
    GLFWwindow* window;
    void (*oninit)(void);
    void (*ontick)(double deltaTime);
    void (*onexit)(void);
} OpenGLExample;

#endif