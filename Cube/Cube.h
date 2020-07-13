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
    std::array<float, 96> cubeMap;
    std::array<char *, 6> textures;
    unsigned int cubeMapTexId;
    unsigned int cubeVAO;
public:
    unsigned int getCubeMapTexId() const;

    unsigned int getCubeVAO() const;

public:
    const std::array<char *, 6> &getTextures() const;

    void setTextures(const std::array<char *, 6> &textures);


    const std::array<float, 96> &getCubeMap() const;


    Cube(float offx, float offy, float offz, std::array<char *, 6> textures, float s = 0.5f);


};


#endif //MAICRA_CUBE_H
