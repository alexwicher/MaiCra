#ifndef MAICRA_RENDERER_H
#define MAICRA_RENDERER_H

#include <array>
#include "../Shader/ShaderLoader.h"
#include "../Camera/Camera.h"

class Renderer {
public:
    std::array<unsigned int, 6> initVertexBuffs(std::array<std::array<float, 20>, 6> cubeMap);

    void renderCube(std::array<unsigned int, 6> VAOList, std::array<unsigned int, 6> texIDsList, ShaderLoader shader,
                    Camera camera);
};


#endif //MAICRA_RENDERER_H
