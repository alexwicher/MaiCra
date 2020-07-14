//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_CUBE_H
#define MAICRA_CUBE_H


#include <array>

class Cube {
private:
    float s = 0.5f;
    float offx, offy, offz;
    std::array<std::array<float, 20>, 6> cubeMap;
    std::array<unsigned int, 6> textures;
public:
    const std::array<unsigned int, 6> &getTextures() const;

private:
    std::array<unsigned int, 6> cubeVAO;
public:
    const std::array<unsigned int, 6> &getCubeVAO() const;

    Cube(float offx, float offy, float offz, std::array<unsigned int, 6> textures, float s = 0.5f);


};


#endif //MAICRA_CUBE_H
