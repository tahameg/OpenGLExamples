#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include "examples.h"

static const GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

static const GLchar* vertexShaderSource = 
    "attribute vec4 position;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = position;\n"
    "}";

static const GLchar* fragmentShaderSource = 
    "precision mediump float;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

static GLuint shaderProgram, vertexShader, fragmentShader, VBO, VAO;

static GLint bIsReady = GL_FALSE; 

static
int linkProgram()
{
    GLint success;

    // Create and compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fprintf(stderr, "Vertex Shader Compilation Error.\n");
        return 0;
    }

    // Create and compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fprintf(stderr, "Fragment Shader Compilation Error.\n");
        return 0;
    }

    // Create the shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        fprintf(stderr, "Shader Program Linking Error.\n");
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 1;
}

static
void init()
{
    if(!linkProgram())
    {
        return;
    }

    bIsReady = GL_TRUE;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static
void tick(double deltaTime)
{
    if(!bIsReady)
    {
        return;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindVertexArray(0);
    glUseProgram(0);
}

static
void exit()
{
    if (bIsReady)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
    bIsReady = GL_FALSE;
}

OpenGLExample SimpleTriangleExample = {
    .name = "Simple Triangle Example",
    .description = "A simple OpenGL example that draws a triangle.",
    .oninit = init,
    .ontick = tick,
    .onexit = exit
};