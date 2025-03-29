#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "CommonFunc.h"

class Player {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;

    Player (int startX, int startY) {
        x = startX;
        y = startY;
        rect = { x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1;
    }

    void move();
    void render(SDL_Renderer* renderer);
};

#endif // PLAYER_H_INCLUDED
