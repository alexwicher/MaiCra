#include "Cube.h"
#include "../Resources/textures/textureTypes.h"


Cube::Cube(const std::array<char *, 6> &cubeType, const glm::vec3 &cubPos) : cubeType(cubeType), cubPos(cubPos) {
    renderFace = {true, true, true, true, true, true};
    for (int i = 0; i < 6; ++i)
        textureArrayIndexs[i] = TEXTURE_INDEX_MAP[cubeType[i]];
    key = std::to_string((int) cubPos.x) + "," + std::to_string((int) cubPos.y) + "," + std::to_string((int) cubPos.z);
}

const std::array<unsigned int, 6> &Cube::getTextureArrayIndexs() const {
    return textureArrayIndexs;
}

