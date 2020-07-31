#include "TextureLoader.h"
#include "../Resources/textures/textureTypes.h"
#include "../Resources/cubes/cubeTypes.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <array>

unsigned int TextureLoader::loadCubeTextures() {
    std::string path = "../Resources/textures/";
    std::string previous;
    int w = 0, h = 0, cha = 0;
    unsigned int texture = 0;
    stbi_info((path + CUBES_TEX_LIST[0]).data(), &w, &h, &cha);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY,
                 0,                // level
                 GL_RGBA8,         // Internal format
                 w, h, CUBES_TEX_LIST.size(), // width,height,depth
                 0,                // border?
                 GL_RGBA,          // format
                 GL_UNSIGNED_BYTE, // type
                 nullptr);               // pointer to data
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    std::array<unsigned char *, CUBES_TEX_LIST.size()> dataArray{};
    for (unsigned int i = 0; i < CUBES_TEX_LIST.size(); i++) {
        TEXTURE_INDEX_MAP[CUBES_TEX_LIST[i]] = i;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load((path + CUBES_TEX_LIST[i]).data(), &w, &h, &cha, STBI_rgb_alpha);
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
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    fillBlockArray();
    for (int j = 0; j < BLOCKS_ARRAYS.size() ; ++j) {
        std::array<unsigned int,6> textureIndexs;
        for (int i = 0; i < BLOCKS_ARRAYS[j].size() ; ++i) {
            textureIndexs[i] = TEXTURE_INDEX_MAP[BLOCKS_ARRAYS[j][i]];
        }
        BLOCK_TEXTURES_IDXS[j] = textureIndexs;
    }

    return texture;

}
