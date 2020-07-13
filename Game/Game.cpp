//
// Created by alex on 7/12/20.
//
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include "Game.h"
#include "../Window/Window.h"
#include "../Renderer/Renderer.h"
#include "../Cube/Cube.h"
#include "../Textures/Texture.h"
#include <iostream>


int Game::startGame() {
    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();

    std::array<char *, 6> texts = {
            const_cast<char *>("../Resources/textures/Dirt.png"),
            const_cast<char *>("../Resources/textures/Dirt.png"),
            const_cast<char *>("../Resources/textures/Dirt.png"),
            const_cast<char *>("../Resources/textures/Dirt.png"),
            const_cast<char *>("../Resources/textures/Dirt.png"),
            const_cast<char *>("../Resources/textures/Dirt.png")
    };

    Cube cube = Cube(0.0f, 0.0f, 0.0f, texts);

    ShaderLoader shader = ShaderLoader(const_cast<char *>("../Shader/rtsShader.vert"),
                                       const_cast<char *>("../Shader/rtsShader.frag"));
    shader.use(); // don't forget to activate/use the shader before setting uniforms!
    shader.setInt("texture1", 0);


    //Main loop ...
    bool loop = true;
    uint32_t ticks, lastticks = 0;

    while (loop) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                loop = false;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        Renderer().renderCube(cube.getCubeVAO(),cube.getCubeMapTexId(),shader);


        ticks = SDL_GetTicks();
        if (((ticks * 10 - lastticks * 10)) < 167) //60 MAX FPS
            SDL_Delay((167 - ((ticks * 10 - lastticks * 10))) / 10);
        lastticks = SDL_GetTicks();

        SDL_GL_SwapWindow(window);
    }
    return 0;

}

int Game::getScreen_width() const {
    return screen_width;
}

void Game::setScreen_width(int screen_width) {
    Game::screen_width = screen_width;
}

int Game::getScreen_height() const {
    return screen_height;
}

void Game::setScreen_height(int screen_height) {
    Game::screen_height = screen_height;
}
