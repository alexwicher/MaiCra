#define STB_IMAGE_IMPLEMENTATION
#include "Cube.h"
#include "../Textures/Texture.h"
#include "../Renderer/Renderer.h"

/*
 * FACES = {
    "right.png",
    "left.png",
    "top.png",
    "bottom.png",
    "front.png",
    "back.png"
    }
 *
 */




Cube::Cube(float offx, float offy, float offz, std::array<char *, 6> textures, float s) : offx(offx), offy(offy),
                                                                                          offz(offz), s(s),
                                                                                          textures(textures) {
    cubeMap = {         //(x,y,z,texCoordX,texCoordY)
            //Front
            -s + offx, s + offy, -s + offz,  //A
            s + offx, s + offy, -s + offz,   //B
            s + offx, -s + offy, -s + offz,  //C
            -s + offx, -s + offy, -s + offz, //D
            // Back
            -s + offx, s + offy, s + offz,
            s + offx, s + offy, s + offz,
            s + offx, -s + offy, s + offz,
            -s + offx, -s + offy, s + offz,
            //Right
            s + offx, s + offy, -s + offz,
            s + offx, s + offy, s + offz,
            s + offx, -s + offy, s + offz,
            s + offx, -s + offy, -s + offz,
            //Left
            -s + offx, s + offy, s + offz,
            -s + offx, s + offy, -s + offz,
            -s + offx, -s + offy, -s + offz,
            -s + offx, -s + offy, s + offz,
            //Top
            -s + offx, s + offy, s + offz,
            s + offx, s + offy, s + offz,
            s + offx, s + offy, -s + offz,
            -s + offx, s + offy, -s + offz,
            //Bottom
            -s + offx, -s + offy, -s + offz,
            s + offx, -s + offy, -s + offz,
            s + offx, -s + offy, s + offz,
            -s + offx, -s + offy, s + offz,
    };

    cubeVAO = Renderer().initVertexBuffs(cubeMap);
    cubeMapTexId = Texture().loadCubemap(textures);

}

const std::array<float, 96> &Cube::getCubeMap() const {
    return cubeMap;
}

const std::array<char *, 6> &Cube::getTextures() const {
    return textures;
}

void Cube::setTextures(const std::array<char *, 6> &textures) {
    Cube::textures = textures;
}

unsigned int Cube::getCubeMapTexId() const {
    return cubeMapTexId;
}

unsigned int Cube::getCubeVAO() const {
    return cubeVAO;
}
