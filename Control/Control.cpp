#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_system.h>
#include <SDL2/SDL.h>
#include <algorithm>
#include "Control.h"
#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"
#include "../gameSettings.h"
#include "../Resources/cubes/cubeTypes.h"

Control::Control() {
    mouseIn = true;
    SDL_ShowCursor(SDL_DISABLE);

}

void
Control::handleControlsEvenst(Camera *camera, Renderer *renderer, bool *loop, SDL_Window *window, float deltaTime) {
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);
    if (keyboard[SDL_SCANCODE_W])
        camera->moveFoward(deltaTime);
    if (keyboard[SDL_SCANCODE_S])
        camera->moveBackward(deltaTime);
    if (keyboard[SDL_SCANCODE_A])
        camera->moveLeft(deltaTime);
    if (keyboard[SDL_SCANCODE_D])
        camera->moveRight(deltaTime);
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
        mouseIn = true;
        if (mouseIn) {
            camera->motion((float) x, (float) y, window);
        }
        switch(event.type){
            case SDL_MOUSEBUTTONDOWN:
                SDL_ShowCursor(SDL_DISABLE);

                if ((SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT))) {
                    renderer->addCube(renderer->getCubeFromMouseRay(camera),STONE_BRICK_BLOCK);
                }
                if ((SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT))) {
                    Cube *cub = renderer->getCubeFromMouseRay(camera)->cubeSelected;
                    if (cub) {
                        renderer->removeCube(cub->key);
                    }
                }
                break;

            default:break;
        }



    }


}
