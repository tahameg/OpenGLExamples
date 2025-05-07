#include <GL/gl.h>
#include <stdio.h>
#include "utils/programUtils.h"

int linkProgram(
    GLshaderSource vertexShaderSource, 
    GLshaderSource fragmentShaderSource, 
    GLuint* program)
{
    GLuint vertexShader, fragmentShader, shaderProgram;
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
    *program = shaderProgram;
    return 1;
}

