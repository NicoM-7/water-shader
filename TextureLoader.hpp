#ifndef TEXTURELOADER_HPP
#define TEXTURELOADER_HPP

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

GLuint loadBMP(const char *imagepath)
{
    printf("Loading image %s\n", imagepath);

    FILE *file = fopen(imagepath, "rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return 0;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }

    int dataPos = *(int *)&(header[0x0A]);
    int imageSize = *(int *)&(header[0x22]);
    int width = *(int *)&(header[0x12]);
    int height = *(int *)&(header[0x16]);

    if (imageSize == 0)
        imageSize = width * height * 3;
    if (dataPos == 0)
        dataPos = 54;

    unsigned char *data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;
    return textureID;
}

#endif