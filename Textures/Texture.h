//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_TEXTURE_H
#define MAICRA_TEXTURE_H


#include <array>

class Texture {
public:
    unsigned int loadTextures();
    unsigned int loadCubemap(std::array<char*, 6> faces);
};


#endif //MAICRA_TEXTURE_H
