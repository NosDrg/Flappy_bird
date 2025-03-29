#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Game.h"

using namespace std;

class Character {
public:
    Character(int x, int z, SDL_Renderer* renderer);
    void handleEvent(const SDL_Event& e);
    void update();
    void render(SDL_Renderer* renderer);
//    void dash(int& mapOffset);
    ~Character();

    const int TILE_MAP = 5;
    int x, z;
    float velocityZ = 0;
    float gravity = 1.2;;
    SDL_Color color;
    const int JUMP_FORCE = -15;
    const int DASH_DISTANCE = 100;
    const int DASH_COOLDOWN = 800;
    int health = 100;
    int lastDashTime = 0;

    bool invincible = false;
    Uint32 invincibleTimer = 0;
    void takeDamage(int damage);

    bool gravityFlip = false;
    bool directionGravity = false;
    Uint32 gravityTimer = 0;
    const int gravityDuration = 5000;
    void updateGravity();

private:
    SDL_Texture* texture;
};
#include "Character.cpp"
#endif
