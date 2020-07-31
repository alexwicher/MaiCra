#ifndef MAICRA_RENDERER_H
#define MAICRA_RENDERER_H

#include <array>
#include <vector>
#include <unordered_map>
#include "../Shader/ShaderLoader.h"
#include "../Camera/Camera.h"
#include "../Cube/Cube.h"

class RaySelection {
public:
    Cube *cubeSelected;
    int faceSelected;
};

class Renderer {
private:
public:
    Renderer(const std::unordered_map<std::string, Cube *> &cubeList);

private:
    std::array<unsigned int, 6> facesVAOs;

    void killUselessNeighbours(Cube *cube);

    void createFace(int x, int y, int z, int side);

    void deleteFace(int x, int y, int z, int side);

    void reloadIBO(unsigned int IBOid, std::vector<glm::vec4> *e);
    int getFaceFromRetroRay(glm::vec3 retroRay);

public:
    std::array<std::vector<glm::vec4>, 6> faceOffsets;
    std::unordered_map<std::string, Cube *> cubeList;
    std::array<unsigned int, 6> facesIBOs;

    void addCube(RaySelection * raySelection,unsigned short cubeType);

    void removeCube(std::string cubeKey);

    void initCubeInstancing(ShaderLoader *shader);

    void renderCubes(unsigned int textureArray, ShaderLoader *shader, Camera *camera);

    RaySelection *getCubeFromMouseRay(Camera *camera);

};


#endif //MAICRA_RENDERER_H
