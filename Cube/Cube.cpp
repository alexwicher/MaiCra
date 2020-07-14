#include "Cube.h"
#include "../Textures/Texture.h"
#include "../Renderer/Renderer.h"

Cube::Cube(float offx, float offy, float offz, std::array<unsigned int, 6> textures, float s) : offx(offx), offy(offy),
                                                                                          offz(offz), s(s),
                                                                                                textures(textures) {
    std::array<float, 20> front = {
            -s + offx, s + offy, -s + offz, 0.0f, 1.0f,  //F-A -s s 0
            s + offx, s + offy, -s + offz, 1.0f, 1.0f,  //F-B s s 1
            s + offx, -s + offy, -s + offz, 1.0f, 0.0f,  //F-C s -s 2
            -s + offx, -s + offy, -s + offz, 0.0f, 0.0f  //F-D -s -s 3
    };
    std::array<float, 20> back = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A -s s 4
            s + offx, s + offy, s + offz, 1.0f, 1.0f,  //B-B s s 5
            s + offx, -s + offy, s + offz, 1.0f, 0.0f, //B-C s- s 6
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f  //B-D -s -s 7
    };
    std::array<float, 20> right = {
            s + offx, s + offy, -s + offz, 0.0f, 1.0f, //F-B
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            s + offx, -s + offy, s + offz, 1.0f, 0.0f, //B-C
            s + offx, -s + offy, -s + offz, 0.0f, 0.0f  //F-C
    };
    std::array<float, 20> left = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A
            -s + offx, s + offy, -s + offz, 1.0f, 1.0f, //F-A
            -s + offx, -s + offy, -s + offz, 1.0f, 0.0f, //F-D
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f  //B-D
    };
    std::array<float, 20> top = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            s + offx, s + offy, -s + offz, 1.0f, 0.0f, //F-B
            -s + offx, s + offy, -s + offz, 0.0f, 0.0f  //F-A
    };
    std::array<float, 20> bottom = {
            -s + offx, -s + offy, -s + offz, 0.0f, 1.0f,//F-D
            s + offx, -s + offy, -s + offz, 1.0f, 1.0f,//F-C
            s + offx, -s + offy, s + offz, 1.0f, 0.0f,//B-C
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f //B-D
    };
    cubeMap = {front, back, right, left, top, bottom};
    cubeVAO = Renderer().initVertexBuffs(cubeMap);
}
const std::array<unsigned int, 6> &Cube::getCubeVAO() const {
    return cubeVAO;
}

const std::array<unsigned int, 6> &Cube::getTextures() const {
    return textures;
}
