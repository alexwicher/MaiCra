#ifndef MAICRA_CONTROL_H
#define MAICRA_CONTROL_H


#include "../Camera/Camera.h"

class Control {
private:
    bool mouseIn;
public:
    Control();

public:
    void handleCamera(Camera * camera,bool * loop, SDL_Window *window, float deltaTime);
};


#endif //MAICRA_CONTROL_H
