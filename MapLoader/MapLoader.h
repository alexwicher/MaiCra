#ifndef MAICRA_MAPLOADER_H
#define MAICRA_MAPLOADER_H


#include "../Cube/Cube.h"

class MapLoader {
public:
    unsigned int* loadMap();
    void renderMap();

private:
    Cube* cubeList;
public:
    Cube *getCubeList() const;

    void setCubeList(Cube *cubeList);

public:
    MapLoader(Cube *cubeList);

private:
    unsigned int* VAOs;
};


#endif //MAICRA_MAPLOADER_H
