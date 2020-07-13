//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_WINDOW_H
#define MAICRA_WINDOW_H

#include <SDL2/SDL.h>


class Window {

public:
    Window(int screen_width, int screen_height);

    int getScreen_width() const;

    void setScreen_width(int screen_width);

    int getScreen_height() const;

    void setScreen_height(int screen_height);

    bool isM_Fullscreen() const;

    void setM_Fullscreen(bool m_Fullscreen);

    SDL_Window *create_window();


private:
    int screen_width, screen_height;
    bool m_Fullscreen = false;


};


#endif //MAICRA_WINDOW_H
