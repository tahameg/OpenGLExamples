#ifndef GLES_TEXTURE_UTILS_H
#define GLES_TEXTURE_UTILS_H

#include <GLES2/gl2.h>
#include <png.h>

int loadPNG(const char* resourcePath,
            unsigned int* outWidth, 
            unsigned int* outHeight, 
            int *outHasAlpha,
            png_byte** outData);

int loadPNGTexture(const char* resourcePath,
    void (*configFunc)(void),
    unsigned int* outWidth, 
    unsigned int* outHeight,
    int *outHasAlpha,
    GLuint* outTextureId);

#endif
