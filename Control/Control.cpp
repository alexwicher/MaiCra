#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL.h>
#include "Control.h"
#include "../Camera/Camera.h"

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
        int x,y;
        Uint32 mouse = SDL_GetMouseState(&x, &y);
        if (mouse == 1) {
            mouseIn = true;
            SDL_ShowCursor(SDL_DISABLE);
        }
        if (mouseIn){
            camera->motion((float) x, (float) y,window);
        }
    }

}

Control::Control(){
    mouseIn = true;
    SDL_ShowCursor(SDL_DISABLE);

}
