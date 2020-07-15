#include <utility>

#include <unordered_map>
#include "MapLoader.h"
#include "../Resources/cubes/cubeTypes.h"
#include "../Renderer/Renderer.h"

void MapLoader::loadDumbMap() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (std::pair<std::string, Cube *> hash : cubeList) {
        Cube *cube = hash.second;
        Renderer().renderCube(cube->getCubeVAO(), cube->getTextures(), mapShader, cam);
    }
}

void MapLoader::handleControls(bool *loop, SDL_Window *window) {
    controls.handleCamera(&cam, loop, window);
}

void MapLoader::initMap(glm::vec3 pos) {

    mapShader.use();
    mapShader.setInt("texture1", 0);
    cam.setPos(pos);
    controls = Control();
    cleanCubes();
    for (std::pair<std::string, Cube *> hash : cubeList) {
        Cube *cube = hash.second;
        cube->initCubeBuffers();
    }

}

MapLoader::MapLoader(Camera cam, ShaderLoader mapShader, std::unordered_map<std::string, Cube *> cubeList) : cam(cam),
                                                                                                             mapShader(
                                                                                                                     mapShader),
                                                                                                             cubeList(
                                                                                                                     std::move(
                                                                                                                             cubeList)) {

}


void MapLoader::cleanCubes() {
    for (std::pair<std::string, Cube *> hash : cubeList) {
        Cube *cube = hash.second;
        float x = cube->offx, y = cube->offy, z = cube->offz, s = cube->s * 2;

        std::array<std::string, 6> checkSurroundings = {
                std::to_string((int)x) + std::to_string((int)y) + std::to_string((int)(z - s)),
                std::to_string((int)x) + std::to_string((int)y) + std::to_string((int)(z + s)),
                std::to_string((int)(x + s)) + std::to_string((int)y) + std::to_string((int)z),
                std::to_string((int)(x - s)) + std::to_string((int)y) + std::to_string((int)z),
                std::to_string((int)x) + std::to_string((int)(y + s)) + std::to_string((int)z),
                std::to_string((int)x) + std::to_string((int)(y - s)) + std::to_string((int)z),
        };

        for (int i = 0; i < 6; ++i) {
            cube->renderFace[i] = cubeList.find(checkSurroundings[i]) == cubeList.end();
        }
    }
}








