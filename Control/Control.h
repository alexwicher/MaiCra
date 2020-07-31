#ifndef MAICRA_CONTROL_H
#define MAICRA_CONTROL_H


#include "../Camera/Camera.h"
#include "../Renderer/Renderer.h"

class Control {
private:
    bool mouseIn;
public:
    Control();
    void handleControlsEvenst(Camera *camera,Renderer * renderer,bool * loop, SDL_Window *window, float deltaTime);
};


#endif //MAICRA_CONTROL_H
