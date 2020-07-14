#ifndef MAICRA_GAME_H
#define MAICRA_GAME_H

class Game {
private:
    int screen_width;
    int screen_height;

public:
    int startGame();

    Game(int sw, int sh) {
        screen_height = sh;
        screen_width = sw;
    }
};

#endif //MAICRA_GAME_H
