#ifndef MAICRA_RENDERER_H
#define MAICRA_RENDERER_H

#include <array>
#include <vector>
#include "../Shader/ShaderLoader.h"
#include "../Camera/Camera.h"
#include "../Cube/Cube.h"
#include "../Control/Control.h"

class Renderer {
private:
    std::array<unsigned int, 6> facesVAOs;
    std::array<std::vector<glm::vec4>, 6> faceOffsets;

public:
    void initCubeInstancing(std::vector<Cube * > cubeList, ShaderLoader * shader);

    void renderCubes(unsigned int textureArray, ShaderLoader *shader,
                     Camera *camera, bool *loop, SDL_Window *window, unsigned int *deltaTime, Control *control);
};


#endif //MAICRA_RENDERER_H
