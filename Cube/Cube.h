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
    std::array<std::array<float,20>, 6> cubeMap;
    std::array<char *, 6> cubeType;
    std::array<unsigned int, 6> texIdList;
    std::array<unsigned int, 6> cubeVAO;
public:
    const std::array<unsigned int, 6> &getTexIdList() const;

    const std::array<unsigned int, 6> &getCubeVAO() const;

    Cube(float offx, float offy, float offz, std::array<char *, 6> cubeType, float s = 0.5f);


};


#endif //MAICRA_CUBE_H
