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

std::string getKey(int x, int y, int z) {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

int Game::startGame() {
    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();
    TextureLoader textureLoader = TextureLoader();
    unsigned int textureArray = textureLoader.loadCubeTextures();
    std::unordered_map<std::string, Cube *> dumbMap;
    Cube pee = Cube(GRASS_BLOCK,glm::vec3(55,5,5));
    int x = 128, y = 8, z = 128;
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < y; ++j) {
            for (int k = 0; k < z; ++k) {
                dumbMap[getKey(i, j, k)] = (new Cube(GRASS_BLOCK, glm::vec3(i, j, k)));
            }
        }
    }
    ShaderLoader shader = ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                       const_cast<char *>("../Shader/rtsShader.frag"));

    Control control = Control();
    Camera camera = Camera(glm::vec3(x/2, y + 1.5, z/2));

    Renderer renderer = Renderer(dumbMap);
    renderer.initCubeInstancing(&shader);
    bool loop = true;
    unsigned int lastFrame = 0, deltaTime = 0;    // time between current frame and last frame
    while (loop) {
        control.handleControlsEvenst(&camera, &renderer, &loop, window, deltaTime);
        glm::vec3 lightDirection = glm::vec3( -0.2f, -1.0f, -0.3f);

        renderer.renderCubes(textureArray, &shader, &camera,lightDirection);



        unsigned int currentFrame = SDL_GetTicks();

//        lightPosition.y = glm::sin(currentFrame / 4000.0f) * (y+1000);
//        lightPosition.x = glm::sin(currentFrame / 4000.0f) * (x/2);

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        SDL_GL_SwapWindow(window);
    }
    return 0;

}

