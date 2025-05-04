#ifndef GLES_PROGRAM_UTILS_H
#define GLES_PROGRAM_UTILS_H 1

typedef const GLchar* const GLshaderSource;

#include <GL/gl.h>

int linkProgram(GLshaderSource vertexShaderSource, GLshaderSource fragmentShaderSource, GLuint* program);

#endif