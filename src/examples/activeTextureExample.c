#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include "examples.h"
#include "config.h"
#include "utils/programUtils.h"
#include "utils/resourceUtils.h"

//for position:
// offset -> 0
// stride -> 9
//for texture:
// offset -> 3
// stride -> 9
//for color:
// offset -> 5
// stride -> 9
static const GLfloat vertices[] = {
  //   vertex  info       texture info       color info
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,    0.0f, 0.0f, 0.2f, 1.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,    0.0f, 0.0f, 0.2f, 1.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,    0.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f
};

static const GLchar* vertexShaderSource = 
    "attribute vec3 aPos;\n"
    "attribute vec2 aTexCoord;\n"
    "attribute vec4 aColor;\n"
    ""
    "varying vec4 ourColor;\n"
    "varying vec2 TexCoord;\n"
    ""
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos, 1.0);\n"
        "ourColor = aColor;\n"
        "TexCoord = aTexCoord;\n"
    "}\n";

static const GLchar* fragmentShaderSource = 
    "precision mediump float;\n"
    ""
    "varying vec4 ourColor;\n"
    "varying vec2 TexCoord;\n"
    ""
    "uniform sampler2D texture0;\n"
    "uniform sampler2D texture1;\n"
    "uniform sampler2D texture2;\n"
    "uniform sampler2D texture3;\n"
    "uniform sampler2D texture4;\n"
    "uniform sampler2D texture5;\n"
    "uniform sampler2D texture6;\n"
    "uniform sampler2D texture7;\n"
    "uniform sampler2D texture8;\n"
    ""
    "void main()\n"
    "{\n"
        "vec4 tex0 = texture2D(texture0, TexCoord);\n"
        "vec4 tex1 = texture2D(texture1, TexCoord);\n"
        "vec4 tex2 = texture2D(texture2, TexCoord);\n"
        "vec4 tex3 = texture2D(texture3, TexCoord);\n"
        "vec4 tex4 = texture2D(texture4, TexCoord);\n"
        "vec4 tex5 = texture2D(texture5, TexCoord);\n"
        "vec4 tex6 = texture2D(texture6, TexCoord);\n"
        "vec4 tex7 = texture2D(texture7, TexCoord);\n"
        "vec4 tex8 = texture2D(texture8, TexCoord);\n"
        "vec4 tex8clean = tex8.w > 0.3 ? tex8 : vec4(0.0, 0.0, 0.0, 0.0);\n"
        "vec4 mixed = (tex0 + tex1 + tex2 + tex3 + tex4 + tex5 + tex6 + tex7 + tex8clean);\n"
        "gl_FragColor = mixed;\n"
    "}\n";

static GLuint shaderProgram, vertexShader, fragmentShader, VBO, VAO;

static GLint bIsReady = GL_FALSE;

static GLuint texIds[9]; // Array to hold 8 texture IDs

static
void init()
{
    if(!linkProgram(vertexShaderSource, fragmentShaderSource, &shaderProgram))
    {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "aPos");
    GLint texAttrib = glGetAttribLocation(shaderProgram, "aTexCoord");
    GLint colorAttrib = glGetAttribLocation(shaderProgram, "aColor");

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(posAttrib);

    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(texAttrib);

    glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(5*sizeof(GLfloat)));
    glEnableVertexAttribArray(colorAttrib);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int width, height, hasAlpha;
    char texturePath[64];
    for (int i = 0; i < 8; i++) 
    {
        snprintf(texturePath, sizeof(texturePath), "/textures/number%d.png", i + 1); 
        if (!loadPNGTexture(texturePath, NULL, &width, &height, &hasAlpha, &texIds[i])) {
            exit(1);
        }
    }

    if (!loadPNGTexture("/textures/spiderman1.png", NULL, &width, &height, &hasAlpha, &texIds[8])) 
    {
        exit(1);
    }

    glUseProgram(shaderProgram);
    for (int i = 0; i < 9; i++) {
        char uniformName[16];
        snprintf(uniformName, sizeof(uniformName), "texture%d", i);
        glUniform1i(glGetUniformLocation(shaderProgram, uniformName), i);
    }
    bIsReady = GL_TRUE;
}

static
void tick(double deltaTime)
{
    if(!bIsReady)
    {
        return;
    }

    static int frames = 0;

    glUseProgram(shaderProgram);

    glClearColor(0.4f, 0.4f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texIds[0]);
    
    for (int i = 1; i < 9; i++) {
        if((frames / (i * 30)) > 0)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, texIds[i]);
        }
    }

    if(frames > 270)
    {
        for (int i = 0; i < 9; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        frames = 0;
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    frames++;
}

static
void exit()
{
    if (bIsReady)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
        glDeleteTextures(8, texIds);
    }
    bIsReady = GL_FALSE;
}

OpenGLExample ActiveTextureExample = {
    .name = "Active Texture Example",
    .description = "An example showcasing glActiveTexture with eight textures.",
    .oninit = init,
    .ontick = tick,
    .onexit = exit
};
