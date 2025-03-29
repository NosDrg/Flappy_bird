#include "Attack.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::vector<Attack> Attack::attacks;

Attack::Attack(int x, int z, int width, int height, int speed, int damage, AttackType type)
    : x(x), z(z), width(width), height(height), speed(speed), damage(damage), active(true), type(type),
      laserState(CHARGING), chargeTime(60), fireTime(40), timer(0) {}

void Attack::update(int score) {
    if (type == PROJECTILE) {
        (speed = 5 + (score / 500 ) < 20) ? (speed = 5 + (score / 500)) : 20;
        x -= speed;
    } else if (type == LASER) {
        timer++;

        if (laserState == CHARGING && timer >= chargeTime) {
            laserState = FIRING;
            timer = 0;
        } else if (laserState == FIRING && timer >= fireTime) {
            active = false;
        }
    }
}

void Attack::render(SDL_Renderer* renderer) {
    if (!active) return;

    if (type == PROJECTILE) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect attackRect = {x, z, width, height};
        SDL_RenderFillRect(renderer, &attackRect);
    } else if (type == LASER) {
        if (laserState == CHARGING) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
            SDL_Rect attackRect = { x, z, width, height };
            SDL_RenderFillRect(renderer, &attackRect);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
        }

        SDL_Rect laserRect = {0, z, 900, 10};
        SDL_RenderFillRect(renderer, &laserRect);
    }
}

bool Attack::checkCollision(Character* character) {
    if (!active) return false;
    if (type == LASER && laserState == CHARGING) return false;

    SDL_Rect attackRect = {x, z, width, height};
    SDL_Rect characterRect = {character->x, character->z, 20, 40};

    if (SDL_HasIntersection(&attackRect, &characterRect)) {
        character->takeDamage(damage);
        active = false;
        return true;
    }
    return false;
}

bool Attack::isOutOfScreen() {
    return !active;
}

void Attack::spawnRandomAttack() {
    int randomZ = rand() % 460;
    int randomSpeed = rand() % 15 + 10 ;
    attacks.emplace_back(900, randomZ, 20, 10, randomSpeed, 10, PROJECTILE);
}

void Attack::spawnLaser() {
    int randomZ = rand() % 460 + 10;
    int laserDamage = 20;
    attacks.emplace_back(0, randomZ, 900, 10, 0, laserDamage, LASER);
}

void Attack::updateAll(Character* character) {
    for (auto& attack : attacks) {
        attack.update(100);
        attack.checkCollision(character);
    }

    attacks.erase(std::remove_if(attacks.begin(), attacks.end(),
                                 [](Attack& attack) { return attack.isOutOfScreen(); }),
                  attacks.end());
}

void Attack::renderAll(SDL_Renderer* renderer) {
    for (auto& attack : attacks) {
        attack.render(renderer);
    }
}
