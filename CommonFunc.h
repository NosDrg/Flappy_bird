#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 480;
const char* WINDOW_TITLE = "BVN";
const int TILE_SIZE = 20;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

using namespace std;

#include "Wall.h"

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;

    void generateWalls() {
    for (int i=3; i < MAP_HEIGHT - 3; i+=2) {
        for (int j = 3; j < MAP_WIDTH - 3; j+=2){
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
            }
        }
    }

    Game();
    void render();
    void run();
    ~Game();

};

#endif // COMMONFUNC_H_INCLUDED
