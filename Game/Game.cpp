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
#include "../Resources/cubes/cubeTypes.h"
#include <iostream>


unsigned int triangulo(); void renderTriangle(ShaderLoader shader, unsigned int tri);

int Game::startGame() {
    Window win = Window(screen_width, screen_height);
    SDL_Window *window = win.create_window();

    Cube cube = Cube(0.0f, 0.0f, 0.0f, GRASS_BLOCK);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer().renderCube(cube.getCubeVAO(), cube.getTexIdList(), shader);

        ticks = SDL_GetTicks();
        if (((ticks * 10 - lastticks * 10)) < 167) //60 MAX FPS
            SDL_Delay((167 - ((ticks * 10 - lastticks * 10))) / 10);
        lastticks = SDL_GetTicks();

        SDL_GL_SwapWindow(window);
    }
    return 0;

}

unsigned int triangulo(){
    std::array<float,20> vertices = {
            0.5f,  0.5f, 0.0f,0.0,1.0,  // top right
            0.5f, -0.5f, 0.0f,1.0,1.0,  // bottom right
            -0.5f, -0.5f, 0.0f,1.0,0.0,  // bottom left
            -0.5f,  0.5f, 0.0f ,0.0,0.0  // top left
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    return VAO;
}

void renderTriangle(ShaderLoader shader, unsigned int tri){
    glBindTexture(GL_TEXTURE, 1);
    shader.use();

    shader.setInt("texture1", 0);

    glBindVertexArray(tri);
    glDrawArrays(GL_POLYGON, 0, 4);
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
