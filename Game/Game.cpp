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
#include "../Resources/cubes/cubeTypes.h"
#include <iostream>
#include <unordered_map>
#include <stb_image.h>

int Game::startGame() {


    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();
    TextureLoader textureLoader = TextureLoader();
    unsigned int textureArray = textureLoader.loadCubeTextures();
    std::vector<Cube *> dumbMap;
    int x = 22, y = 2, z = 22;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            for (int k = 0; k < z; ++k) {
                dumbMap.emplace_back(new Cube(GRASS_BLOCK, glm::vec3(i, j, k)));
            }
        }
    }
    ShaderLoader shader = ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                       const_cast<char *>("../Shader/rtsShader.frag"));


    Control control = Control();
    Camera camera = Camera(glm::vec3(0, 0, 0));
    Renderer renderCubes = Renderer();
    renderCubes.initCubeInstancing(dumbMap, &shader);
    bool loop = true;
    unsigned int lastFrame = 0, deltaTime = 0;    // time between current frame and last frame
    while (loop) {
        renderCubes.renderCubes(textureArray, &shader, &camera, &loop, window, &deltaTime, &control);
        unsigned int currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        SDL_GL_SwapWindow(window);
    }
    return 0;

}

