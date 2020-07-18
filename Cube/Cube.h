#ifndef MAICRA_CUBE_H
#define MAICRA_CUBE_H


#include <array>
#include <glm/vec3.hpp>

class Cube {
private:
    std::array<bool, 6> renderFace;
    std::array<unsigned int, 6> textureArrayIndexs;


public:
    std::array<char *, 6> cubeType;

    Cube(const std::array<char *, 6> &cubeType, const glm::vec3 &cubPos);


    const std::array<bool, 6> &getRenderFace() const;

    void setRenderFace(const std::array<bool, 6> &renderFace);

    const std::array<unsigned int, 6> &getTextureArrayIndexs() const;

    glm::vec3 cubPos;


};


#endif //MAICRA_CUBE_H
