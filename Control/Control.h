#ifndef MAICRA_CONTROL_H
#define MAICRA_CONTROL_H


class Control {
private:
    bool mouseIn = false;
public:
    Control();
public:
    void handleKeyBoard();
    void handleMouse(SDL_Window *win);

};


#endif //MAICRA_CONTROL_H
