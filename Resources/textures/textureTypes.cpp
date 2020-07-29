#include <array>
#include "textureTypes.h"

std::unordered_map<std::string,unsigned int> TEXTURE_INDEX_MAP;
std::array<const char *,10> CUBES_TEX_LIST={DIRT,GRASS,GRASS_SIDE,ONE,TWO,THREE,FOUR,FIVE,SIX,STONE};

char *DIRT = const_cast<char *>("Dirt.png");
char *GRASS = const_cast<char *>("Grass.png");
char *GRASS_SIDE = const_cast<char *>("GrassSide.png");

char *ONE = const_cast<char *>("ONE.png");
char *TWO = const_cast<char *>("TWO.png");
char *THREE = const_cast<char *>("THREE.png");
char *FOUR = const_cast<char *>("FOUR.png");
char *FIVE = const_cast<char *>("FIVE.png");
char *SIX = const_cast<char *>("SIX.png");

char *STONE = const_cast<char *>("stone.png");