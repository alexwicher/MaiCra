#ifndef MAICRA_CUBE_H
#define MAICRA_CUBE_H


#include <array>
#include <glm/vec3.hpp>

class Cube {
public:
    unsigned char renderFace;
    unsigned short cubeType;
    Cube(unsigned short cubeType, const glm::vec3 &cubPos);

    const std::array<unsigned int, 6> &getTextureArrayIndexs() const;

    glm::vec3 cubPos;
    std::string getKey();
    bool shouldRenderFace(int face);
    void setRenderFace(int face,bool set);
};


#endif //MAICRA_CUBE_H
