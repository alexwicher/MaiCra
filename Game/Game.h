//
// Created by alex on 7/12/20.
//

#ifndef MAICRA_GAME_H
#define MAICRA_GAME_H


class Game {
private:
    int screen_width;
    int screen_height;

public:
    int startGame();
    Game(int sw,int sh){
        screen_height = sh;
        screen_width = sw;
    }

    int getScreen_width() const;

    void setScreen_width(int screen_width);

    int getScreen_height() const;

    void setScreen_height(int screen_height);
};



#endif //MAICRA_GAME_H
