#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../Cube/Cube.h"
#include "../Textures/Texture.h"
#include "../Camera/Camera.h"
#include "../gameSettings.h"
#include "../Control/Control.h"
#include "../MapLoader/MapLoader.h"
#include "../Resources/cubes/cubeTypes.h"
#include <iostream>

int Game::startGame() {
    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();
    Cube * flatRetardedMap = (Cube*)malloc(5*5* sizeof(Cube));
    std::array<unsigned int, 6> textures = Texture().loadCube(GRASS_BLOCK);
    int x = 15, y = 15;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            flatRetardedMap[i*x + j] = Cube(i, 0, j, textures);
        }
    }
    MapLoader dumbMap = MapLoader(Camera(glm::vec3(0, 0, 0)),
                                  ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                               const_cast<char *>("../Shader/rtsShader.frag")),
                                  flatRetardedMap,x,y);

    dumbMap.initMap(glm::vec3(0, 0, 0));


    //Main loop ...
    bool loop = true;
    uint32_t ticks, lastticks = 0;
    while (loop) {

        dumbMap.handleControls(&loop, window);
        dumbMap.loadDumbMap(5, 5);

        ticks = SDL_GetTicks();
        if (((ticks * 10 - lastticks * 10)) < 167) //60 MAX FPS
            SDL_Delay((167 - ((ticks * 10 - lastticks * 10))) / 10);
        lastticks = SDL_GetTicks();

        SDL_GL_SwapWindow(window);
    }
    return 0;

}

