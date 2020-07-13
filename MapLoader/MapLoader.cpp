#include "MapLoader.h"

unsigned int *MapLoader::loadMap() {
    return nullptr;
}

void MapLoader::renderMap() {

}




MapLoader::MapLoader(Cube *cubeList) : cubeList(cubeList) {}

Cube *MapLoader::getCubeList() const {
    return cubeList;
}

void MapLoader::setCubeList(Cube *cubeList) {
    MapLoader::cubeList = cubeList;
}
