#include "Cube.h"
#include "../TextureLoader/TextureLoader.h"
#include "../Renderer/Renderer.h"

Cube::Cube(float offx, float offy, float offz, std::array<unsigned int, 6> textures, float s) : offx(offx), offy(offy),
                                                                                                offz(offz), s(s),
                                                                                                textures(textures) {

    renderFace = {true, true, true, true, true, true};
    std::array<float, 36> front = {
            -s + offx, s + offy, -s + offz, 0.0f, 1.0f,  //F-A -s s 0
            s + offx, s + offy, -s + offz, 1.0f, 1.0f,  //F-B s s 1
            -s + offx, -s + offy, -s + offz, 0.0f, 0.0f,  //F-D -s -s 3
            s + offx, s + offy, -s + offz, 1.0f, 1.0f,  //F-B s s 1
            s + offx, -s + offy, -s + offz, 1.0f, 0.0f,  //F-C s -s 2
            -s + offx, -s + offy, -s + offz, 0.0f, 0.0f  //F-D -s -s 3
    };
    std::array<float, 36> back = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A -s s 4
            s + offx, s + offy, s + offz, 1.0f, 1.0f,  //B-B s s 5
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f,  //B-D -s -s 7
            s + offx, s + offy, s + offz, 1.0f, 1.0f,  //B-B s s 5
            s + offx, -s + offy, s + offz, 1.0f, 0.0f, //B-C s- s 6
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f  //B-D -s -s 7
    };
    std::array<float, 36> right = {
            s + offx, s + offy, -s + offz, 0.0f, 1.0f, //F-B
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            s + offx, -s + offy, -s + offz, 0.0f, 0.0f,  //F-C
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            s + offx, -s + offy, s + offz, 1.0f, 0.0f, //B-C
            s + offx, -s + offy, -s + offz, 0.0f, 0.0f  //F-C
    };
    std::array<float, 36> left = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A
            -s + offx, s + offy, -s + offz, 1.0f, 1.0f, //F-A
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f,  //B-D
            -s + offx, s + offy, -s + offz, 1.0f, 1.0f, //F-A
            -s + offx, -s + offy, -s + offz, 1.0f, 0.0f, //F-D
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f  //B-D
    };
    std::array<float, 36> top = {
            -s + offx, s + offy, s + offz, 0.0f, 1.0f, //B-A
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            -s + offx, s + offy, -s + offz, 0.0f, 0.0f,  //F-A
            s + offx, s + offy, s + offz, 1.0f, 1.0f, //B-B
            s + offx, s + offy, -s + offz, 1.0f, 0.0f, //F-B
            -s + offx, s + offy, -s + offz, 0.0f, 0.0f  //F-A
    };
    std::array<float, 36> bottom = {
            -s + offx, -s + offy, -s + offz, 0.0f, 1.0f,//F-D
            s + offx, -s + offy, -s + offz, 1.0f, 1.0f,//F-C
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f, //B-D
            s + offx, -s + offy, -s + offz, 1.0f, 1.0f,//F-C
            s + offx, -s + offy, s + offz, 1.0f, 0.0f,//B-C
            -s + offx, -s + offy, s + offz, 0.0f, 0.0f //B-D
    };
    cubeMap = {front, back, right, left, top, bottom};
}

const std::array<unsigned int, 6> &Cube::getCubeVAO() const {
    return cubeVAO;
}

const std::array<unsigned int, 6> &Cube::getTextures() const {
    return textures;
}

void Cube::initCubeBuffers() {
    for (int i = 0; i < cubeMap.size(); ++i) {
        if (renderFace[i])
            cubeVAO[i] = initVertexBuffs(cubeMap[i]);
        else
            cubeVAO[i] = 0;
    }
}

unsigned int Cube::initVertexBuffs(std::array<float, 36> squareMap) {
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, squareMap.size() * sizeof(float), &squareMap[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    return VAO;
}

