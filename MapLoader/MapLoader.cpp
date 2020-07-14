#include "MapLoader.h"
#include "../Resources/cubes/cubeTypes.h"
#include "../Renderer/Renderer.h"

void MapLoader::loadDumbMap(int x, int y) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < mapX*mapY; i++) {
        Cube cube = cubeList[i];
        Renderer().renderCube(cube.getCubeVAO(), cube.getTextures(), mapShader, cam);
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
}

MapLoader::MapLoader(Camera cam, ShaderLoader mapShader, Cube *cubeList, int mapX, int mapY) : cam(cam),
                                                                                               mapShader(mapShader),
                                                                                               cubeList(cubeList),
                                                                                               mapX(mapX), mapY(mapY) {

}








