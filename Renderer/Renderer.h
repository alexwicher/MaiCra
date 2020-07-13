//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_RENDERER_H
#define MAICRA_RENDERER_H


#include <array>
#include "../Shader/ShaderLoader.h"

class Renderer {
public:
    unsigned int initVertexBuffs(std::array<float, 96> cubeMap);

    void renderCube(unsigned int VAO, unsigned cubeTexID, ShaderLoader shader);
};


#endif //MAICRA_RENDERER_H
