#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "config.h"

int loadPNG(const char* filePath,
            unsigned int* outWidth, 
            unsigned int* outHeight,
            int* outHasAlpha,
            png_bytepp* outData)
{
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fprintf(stderr, "Failed to create PNG read struct.\n");
        return 0;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_read_struct(&png, NULL, NULL);
        fprintf(stderr, "Failed to create PNG info struct.\n");
        return 0;
    }
    char abs_path[350] = RESOURCES_DIR;
    strcat(abs_path, filePath);

    FILE* file = fopen(abs_path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to open texture file: %s\n", abs_path);
        return 0;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        fprintf(stderr, "Error during PNG read.\n");
        return 0;
    }

    //png_set_sig_bytes(png, 0);
    png_init_io(png, file);
    png_read_info(png, info);

    unsigned int width  = png_get_image_width(png, info);
    unsigned int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);
  

    *outHasAlpha = (color_type == PNG_COLOR_TYPE_RGBA);
    *outWidth = width;
    *outHeight = height;

    if(bit_depth == 16)
    png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_byte** row_pointers = (png_byte**)malloc(sizeof(png_byte*) * height);
    if(!row_pointers)
    {
        longjmp(png_jmpbuf(png), 1);
        return;
    }

    size_t row_byte_count = png_get_rowbytes(png,info);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(row_byte_count);
    }

    png_read_image(png, row_pointers);

    /*
    for (int i = 0; i < (height / 2); i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        png_byte* temp = row_pointers[i];
        row_pointers[i] = row_pointers[height - i - 1];
        row_pointers[height - i - 1] = temp;
    }
    */

    *outData = row_pointers;
    png_destroy_read_struct(&png, &info, NULL);
    fclose(file);
    return 1;
}

int loadPNGTexture(const char* filePath,
                   unsigned int* outWidth, 
                   unsigned int* outHeight,
                   GLuint* outTextureId)
{
    png_bytepp imageData;
    int hasAlpha = 0;
    if(!loadPNG(filePath, outWidth, outHeight, &hasAlpha, &imageData))
    {
        return 0;
    }
    
    glGenTextures(1, outTextureId);
    glBindTexture(GL_TEXTURE_2D, *outTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *outWidth, *outHeight, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, imageData);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for(int i = 0; i < *outHeight; i++)
    {
        free(imageData[i]);
    }
    free(imageData);

    return 1;
}