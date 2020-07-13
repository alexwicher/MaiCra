#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>

#include "Control.h"
#include "../gameSettings.h"


void Control::handleKeyBoard() {

    SDL_PumpEvents();
    const Uint8 *keyboard = SDL_GetKeyboardState(nullptr);

    if (keyboard[SDL_SCANCODE_W]) {

    } else if (keyboard[SDL_SCANCODE_S]) {

    } else if (keyboard[SDL_SCANCODE_A]) {

    } else if (keyboard[SDL_SCANCODE_D]) {

    } else if (keyboard[SDL_SCANCODE_F12]) {
        SDL_Quit();
    } else if (keyboard[SDL_SCANCODE_DELETE]){
        mouseIn = false;
        SDL_ShowCursor(SDL_ENABLE);
    }


}

void Control::handleMouse(SDL_Window *window) {
    int x , y;
    SDL_PumpEvents();

    Uint32 mouse = SDL_GetMouseState(&x,&y);

    if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        mouseIn = true;
        SDL_ShowCursor(SDL_DISABLE);
    }


    if(mouseIn){

        SDL_WarpMouseInWindow(window,SCR_WIDTH/2,SCR_HEIGHT/2);
    }
}

Control::Control() {
    mouseIn = true;
    SDL_ShowCursor(SDL_ENABLE);

}
