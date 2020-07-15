#ifndef MAICRA_MAPLOADER_H
#define MAICRA_MAPLOADER_H

#include <vector>
#include <unordered_map>
#include "../Cube/Cube.h"
#include "../Camera/Camera.h"
#include "../Shader/ShaderLoader.h"
#include "../Control/Control.h"

class MapLoader {

private:
    glm::vec3 pos;
    Control controls;
    Camera cam;
    ShaderLoader mapShader;
    std::unordered_map<std::string, Cube *> cubeList;
    int mapX, mapY, mapZ;

    void cleanCubes();

public:
    MapLoader(Camera cam, ShaderLoader mapShader, std::unordered_map<std::string, Cube *> cubeList);

    void loadDumbMap();

    void initMap(glm::vec3 pos);

    void handleControls(bool *loop, SDL_Window *window);

};

#endif //MAICRA_MAPLOADER_H
