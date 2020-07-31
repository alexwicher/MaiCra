#include "cubeTypes.h"


std::array<std::array<unsigned int, 6>, size> BLOCK_TEXTURES_IDXS;
std::array<std::array<char *, 6>, size> BLOCKS_ARRAYS;

unsigned short GRASS_BLOCK = 0;
unsigned short STONE_BRICK_BLOCK = 1;

void fillBlockArray() {
    BLOCKS_ARRAYS[GRASS_BLOCK] = {GRASS_SIDE,
                                  GRASS_SIDE,
                                  GRASS_SIDE,
                                  GRASS_SIDE,
                                  GRASS,
                                  DIRT};
    BLOCKS_ARRAYS[STONE_BRICK_BLOCK] = {STONE_BRICK,
                                        STONE_BRICK,
                                        STONE_BRICK,
                                        STONE_BRICK,
                                        STONE_BRICK,
                                        STONE_BRICK};
}