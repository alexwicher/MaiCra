#include <array>
#include "../textures/textureTypes.h"

//Front
//Back
//Right
//Left
//Top
//Bottom
const int size = 2;
extern std::array<std::array<unsigned int,6>,size> BLOCK_TEXTURES_IDXS;
extern std::array<std::array<char *, 6>,size> BLOCKS_ARRAYS;

extern unsigned short GRASS_BLOCK;
extern unsigned short STONE_BRICK_BLOCK;
void fillBlockArray();