//
// Created by alex on 7/12/20.
//

#include "TextureLoader.h"
#include "../Resources/textures/textureTypes.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <array>
void glErrors() {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        std::cout << error << std::endl;
    }
}

unsigned int TextureLoader::loadCubeTextures() {
    std::string path = "../Resources/textures/";
    std::string previous;
    int w = 0, h = 0, cha = 0;
    unsigned int texture = 0;
    stbi_info((path+TEX_LIST[0]).data(), &w, &h, &cha);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                // level
                 GL_RGBA8,         // Internal format
                 w, h, TEX_LIST.size(), // width,height,depth
                 0,                // border?
                 GL_RGBA,          // format
                 GL_UNSIGNED_BYTE, // type
                 nullptr);               // pointer to data


    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    std::array<unsigned char *, TEX_LIST.size()> dataArray{};
    for (int i = 0;i<TEX_LIST.size();i++) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load((path+TEX_LIST[i]).data(), &w, &h, &cha, STBI_rgb_alpha);
        if (data) {
                glTexSubImage3D(GL_TEXTURE_2D_ARRAY,    // GL_TEXTURE_2D_ARRAY
                    0,                    // mipmap level to load
                    0,                    // offset in an existing texture to load to
                    0,                    // offset in an existing texture to load to
                    i,                    // first array slice to load to (zoffset)
                    w,                    // width of data to be loaded
                    h,                    // height of data to be loaded
                    1,                    // number of array slices to load
                    GL_RGBA,                    // format of data to be loaded
                    GL_UNSIGNED_BYTE,                    // type of data to be loaded
                    data);         // pointer ro data to be loaded.    glErrors();
            glErrors();

        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);


    return texture;

}
