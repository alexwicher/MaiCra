//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_CUBE_H
#define MAICRA_CUBE_H


#include <array>
#include <glm/vec3.hpp>

class Cube {
private:
    std::array<unsigned int, 6> cubeVAO;

    std::array<std::array<float, 30>, 6> cubeMap;
    std::array<unsigned int, 6> textures;

    unsigned int initVertexBuffs(std::array<float, 30> squareMap);

public:
    float offx, offy, offz, s = 0.5f;
    std::array<bool, 6> renderFace;


    void initCubeBuffers();

    const std::array<std::array<float, 30>, 6> &getCubeMap() const;

    const std::array<unsigned int, 6> &getTextures() const;

    const std::array<unsigned int, 6> &getCubeVAO() const;

    Cube(float offx, float offy, float offz, std::array<unsigned int, 6> textures, float s = 0.5f);


};


#endif //MAICRA_CUBE_H
