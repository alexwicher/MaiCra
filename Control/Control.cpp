#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include "Control.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"
#include "../gameSettings.h"
#include "../Resources/cubes/cubeTypes.h"

void Control::handleCamera(Camera *camera, bool *loop, SDL_Window *window, float deltaTime) {
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_W]) {
        camera->moveFoward(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_S]) {
        camera->moveBackward(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_A]) {
        camera->moveLeft(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_D]) {
        camera->moveRight(deltaTime);
    }
    if (keyboard[SDL_SCANCODE_F12]) {
        *loop = false;
        SDL_Quit();
    }
    if (keyboard[SDL_SCANCODE_DELETE]) {
        mouseIn = false;
        SDL_ShowCursor(SDL_ENABLE);
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        int x, y;
        Uint32 mouse = SDL_GetMouseState(&x, &y);
        if (mouse == 1) {
            mouseIn = true;
            SDL_ShowCursor(SDL_DISABLE);
        }
        if (mouseIn) {
            camera->motion((float) x, (float) y, window);
        }
    }

}

void Control::handleRenderer(Renderer *renderer) {
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_E]) {
        std::string key = "0,3,0";
        if (renderer->cubeList.find(key) != renderer->cubeList.end())
            renderer->removeCube(renderer->cubeList[key]);

    }
    if (keyboard[SDL_SCANCODE_Q]) {
        std::string key = "0,3,0";
        if (renderer->cubeList.find(key) == renderer->cubeList.end())
            renderer->addCube(new Cube(STONE_BLOCK,glm::vec3(0,3,0)));
    }
}

Control::Control() {
    mouseIn = true;
    SDL_ShowCursor(SDL_DISABLE);

}
