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
    std::unordered_map<std::string, Cube *> flatRetardedMap;
    std::array<unsigned int, 6> grassTextures = TextureLoader().loadCube(GRASS_BLOCK);
    int x = 64, y = 2, z = 64;
    for (int i = 0; i < x; ++i) {
        for (int k = 0; k < y; ++k) {
            for (int j = 0; j < z; ++j) {
                flatRetardedMap[std::to_string(i) + "x" + std::to_string(k) + "y" + std::to_string(j) + "z"] = new Cube(
                        i, k, j, grassTextures);
            }
        }
    }
    flatRetardedMap[std::to_string(x / 2 - 1) + "x" + std::to_string(y + 2) + "y" + std::to_string(z / 2 - 1) +
                    "z"] = new Cube(x / 2, y + 2, z / 2, grassTextures);
    MapLoader dumbMap = MapLoader(Camera(glm::vec3(0, 0, 0)),
                                  ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                               const_cast<char *>("../Shader/rtsShader.frag")),
                                  flatRetardedMap);

    dumbMap.initMap(glm::vec3(x / 2, y + 2, z / 2));

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Activate for lines
    //Main loop ...
    bool loop = true;
    unsigned int lastFrame = 0, deltaTime = 0;    // time between current frame and last frame
    while (loop) {
        unsigned int currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        dumbMap.handleControls(&loop, window, deltaTime);
        dumbMap.loadMap();


        SDL_GL_SwapWindow(window);
    }
    return 0;

}

