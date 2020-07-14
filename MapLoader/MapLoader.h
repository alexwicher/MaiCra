#ifndef MAICRA_MAPLOADER_H
#define MAICRA_MAPLOADER_H

#include <vector>
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
    Cube * cubeList;
    int mapX,mapY;
public:
    MapLoader(Camera cam, ShaderLoader mapShader,Cube * cubeList,int mapY,int mapX);
    void loadDumbMap(int x, int y);
    void initMap(glm::vec3 pos);
    void handleControls(bool *  loop,SDL_Window * window);

};

#endif //MAICRA_MAPLOADER_H
