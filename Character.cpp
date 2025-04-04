#include "Character.h"
#include "Game.h"

Character::Character(int x, int z, SDL_Renderer* renderer) : x(x), z(z), health(100), invincible(false), gravityFlip(false), velocityZ(0), directionGravity(false) {
    if (!texture) {
        SDL_Log("Failed to load character texture: %s", IMG_GetError());
    }
}

void Character::setSize(int w, int h){
    width = w;
    height = h;

}

void Character::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if(!gravityFlip) {
                    velocityZ = JUMP_FORCE;
                }else{
                    ( directionGravity == false ) ? directionGravity = true : directionGravity = false;
                }
                break;

        }
    }
}

void Character::update() {
    if(!gravityFlip) {
        velocityZ += gravity;
        z += velocityZ;
    }else{
        if(directionGravity){
            z += gravity * 5;
        }else{

            z -= gravity * 5;
        }
    }
    if (z < 0 ) z = 0 ;
    if (z >= 440) {
        z = 440;
        velocityZ = 0;
    }
     if (invincible && SDL_GetTicks() - invincibleTimer > 3000) {
        invincible = false;
    }
    if (gravityFlip && SDL_GetTicks() - gravityTimer > 5000) {
        gravityFlip = false;
    }
}
/*
void Character::dash(int& mapOffset) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastDashTime >= DASH_COOLDOWN) {
        mapOffset -= DASH_DISTANCE;
        lastDashTime = currentTime;
    }
}
*/
void Character::takeDamage(int damage) {
    if (invincible) return;

    health -= damage;
    if (health <= 0) {
        health = 0;

    }
}

void Character::updateGravity() {
    if( gravityFlip && SDL_GetTicks() - gravityTimer > gravityDuration) {
        gravityFlip = false;
    }
}

void Character::render(SDL_Renderer* renderer) {
    if (invincible) {
        texture = IMG_LoadTexture(renderer, "character//Ship2.png");
    }
    if(gravityFlip) {
        texture = IMG_LoadTexture(renderer, "character//Ship3.png");
    }
    if(!invincible && !gravityFlip) {
        texture = IMG_LoadTexture(renderer, "character//Ship1.png");

    }

    SDL_Rect characterRect = {x, z, width * 2 , height * 2 };
    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &characterRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &characterRect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect healthBar = { x + 10, z - 15, health / 2, 5};
    SDL_RenderFillRect(renderer, &healthBar);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect emptyBar = { x + 10 + health / 2, z - 15, (100 - health) / 2, 5};
    SDL_RenderFillRect(renderer, &emptyBar);

}

Character::~Character() {
if(texture){
        SDL_DestroyTexture(texture);
    }
}
