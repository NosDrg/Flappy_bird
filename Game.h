#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Character.h"
#include "Attack.h"
#include "Item.h"

using namespace std;

class Game {
public:
    const int SCREEN_WIDTH = 900;
    const int SCREEN_HEIGHT = 480;
    const char* TITLE = "Flappy bird";
    const int TILE_MAP = 5;
    const int MAX_TILE_WIDTH = SCREEN_WIDTH / TILE_MAP;
    const int MAX_TILE_HEIGHT = SCREEN_HEIGHT / TILE_MAP;

    Game();
    bool init();
    void run();
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);
    void scoreGame(int value);
    void close();
    void renderMenu(SDL_Renderer* renderer, SDL_Texture* menu);
    void renderEnd();
    ~Game();

    int score = 0;
    int mapOffset;
    bool inMenu = true;
    bool endgame = false;
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Texture* mapTexture;
    SDL_Texture* menuTexture;
    SDL_Texture* endTexture;
    bool running;
    Character* player1;

//    TTF_Font* font;

    vector<SDL_Texture*> mapTextures;
    int currentMapIndex;

    static const int TOTAL_MAPS = 5;
    float mapSpeed;

    int mapWidth;
    int mapHeight;

    int highScore;

    void processInput();
    void update();
    void render();
    void loadMaps();
    void switchToNextMap();
    //void Gameover();
    //void resetGame();
};
#include "Game.cpp"

#endif
