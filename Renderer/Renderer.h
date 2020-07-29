#ifndef MAICRA_RENDERER_H
#define MAICRA_RENDERER_H

#include <array>
#include <vector>
#include <unordered_map>
#include "../Shader/ShaderLoader.h"
#include "../Camera/Camera.h"
#include "../Cube/Cube.h"

class Renderer {
private:
public:
    Renderer(const std::unordered_map<std::string, Cube *> &cubeList);

private:
    std::array<unsigned int, 6> facesVAOs;

    void killUselessNeighbours(std::unordered_map<std::string, Cube *> cubeList);

    void createFace(int x, int y ,int z,int side);

    void deleteFace(glm::vec3 pos,int side);

    void reloadIBO(unsigned int IBOid, std::vector<glm::vec4> *e);

public:
    std::array<std::vector<glm::vec4>, 6> faceOffsets;
    std::unordered_map<std::string, Cube *> cubeList;
    std::array<unsigned int, 6> facesIBOs;

    void addCube(Cube *cube);

    void removeCube(Cube *cube);

    void initCubeInstancing(ShaderLoader *shader);

    void renderCubes(unsigned int textureArray, ShaderLoader *shader, Camera *camera);
};


#endif //MAICRA_RENDERER_H
