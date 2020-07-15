#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../Cube/Cube.h"
#include "../TextureLoader/TextureLoader.h"
#include "../Camera/Camera.h"
#include "../gameSettings.h"
#include "../Control/Control.h"
#include "../MapLoader/MapLoader.h"
#include "../Resources/cubes/cubeTypes.h"
#include <iostream>
#include <unordered_map>

int Game::startGame() {


    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();
    std::unordered_map<std::string, Cube*> flatRetardedMap;
    std::array<unsigned int, 6> textures = TextureLoader().loadCube(GRASS_BLOCK);

    int x = 80, y = 1, z =80;
    for (int i = 0; i < x; ++i) {
        for (int k = 0; k < y; ++k) {
            for (int j = 0; j < z; ++j) {
                flatRetardedMap[std::to_string(i) + std::to_string(k) + std::to_string(j)] = new Cube(i, k, j, textures);
            }
        }
    }
    MapLoader dumbMap = MapLoader(Camera(glm::vec3(0, 0, 0)),
                                  ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                               const_cast<char *>("../Shader/rtsShader.frag")),
                                  flatRetardedMap);

    dumbMap.initMap(glm::vec3(0, 0, 0));


    //Main loop ...
    bool loop = true;
    uint32_t ticks, lastticks = 0;
    while (loop) {

        dumbMap.handleControls(&loop, window);
        dumbMap.loadDumbMap();

        ticks = SDL_GetTicks();
        if (((ticks * 10 - lastticks * 10)) < 167) //60 MAX FPS
            SDL_Delay((167 - ((ticks * 10 - lastticks * 10))) / 10);
        lastticks = SDL_GetTicks();

        SDL_GL_SwapWindow(window);
    }
    return 0;

}

