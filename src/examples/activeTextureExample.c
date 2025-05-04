#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include "examples.h"
#include "config.h"
#include "utils/programUtils.h"
#include "utils/resourceUtils.h"

static const GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

static const GLfloat texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
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
    "    gl_FragColor = vec4(6.0, 0.2, 4.0, 1.0);\n"
    "}\n";

static GLuint shaderProgram, vertexShader, fragmentShader, VBO, VAO;

static GLint bIsReady = GL_FALSE; 

static
void init()
{
    if(!linkProgram(vertexShaderSource, fragmentShaderSource, &shaderProgram))
    {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    char** data;
    unsigned int width, height, isAlpha;
    if(!loadPNG("/textures/spiderman1.png", &width, &height, &isAlpha, &data))
    {
        return;
    }

    for(int i = 0; i < height; i+=6)
    {
        for(int j = 0; j < width; j+=6)
        {
            char alpha = data[i][(j*(isAlpha ? 4 : 3) - 1)];
            putchar(alpha ? ' ' : '#');
        }
        putchar('\n');
    }
    printf("PNG is loaded: \n" 
            "\twidth -> %d \n"
            "\theight -> %d \n"
            "\tisAlpha -> %s \n", width, height, isAlpha ? "true" : "false");
    free(data);
}

static
void tick(double deltaTime)
{
    if(!bIsReady)
    {
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

OpenGLExample ActiveTextureExample = {
    .name = "Active Texture Example",
    .description = "An example showcasing glActiveTexture with two textures.",
    .oninit = init,
    .ontick = tick,
    .onexit = exit
};
