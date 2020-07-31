#include "Cube.h"
#include "../Resources/textures/textureTypes.h"
#include "../Resources/cubes/cubeTypes.h"

#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))

Cube::Cube(const unsigned short cubeType, const glm::vec3 &cubPos) : cubeType(cubeType), cubPos(cubPos) {
    renderFace = 0b00111111;

}

const std::array<unsigned int, 6> &Cube::getTextureArrayIndexs() const {
    return BLOCK_TEXTURES_IDXS[cubeType];
}

std::string Cube::getKey() {
    return std::to_string((int) cubPos.x) + "," + std::to_string((int) cubPos.y) + "," + std::to_string((int) cubPos.z);
}

bool Cube::shouldRenderFace(int face) {
    return (bool) CHECK_BIT(renderFace, face);
}

void Cube::setRenderFace(int face,bool set) {
    if (set)
        renderFace |= 1 << face;
    else
        renderFace &= ~(1 << face);
}

