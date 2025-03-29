#include "Item.h"
#include <cstdlib>
#include <ctime>

std::vector<Item> Item::items;

Item::Item(int x, int z, int size, ItemType type)
    : x(x), z(z), size(size), type(type), active(true) {}

void Item::update() {
    x -= 5;
    if (x < -size) {
        active = false;
    }
}

void Item::render(SDL_Renderer* renderer) {
    if (!active) return;

    if (type == HEALTH) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
         // Hp
    } else if (type == INVINCIBILITY) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
         // Shield
    } else if (type == GRAVITY_SWAP) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
         // Gravity Swap
    }

    SDL_Rect itemRect = {x, z, size, size};
    SDL_RenderFillRect(renderer, &itemRect);
}

bool Item::checkCollision(Character* character) {
    if (!active) return false;

    SDL_Rect itemRect = {x, z, size, size};
    SDL_Rect characterRect = {character->x, character->z, 20, 40};

    if (SDL_HasIntersection(&itemRect, &characterRect)) {
        if (type == HEALTH) {
            character->health = std::min(100, character->health + 20);
        } else if (type == INVINCIBILITY) {
            character->invincible = true;
            character->invincibleTimer = SDL_GetTicks();
        } else if (type == GRAVITY_SWAP) {
            character->gravityFlip = true;
            character->gravityTimer = SDL_GetTicks();
        }

        active = false;
        return true;
    }
    return false;
}

bool Item::isOutOfScreen() {
    return !active;
}

void Item::spawnRandomItem() {
    int randomZ = rand() % 400 + 40;
    int randType = rand() % 100;
    ItemType randomType = (randType < 40) ? HEALTH : (randType < 80) ? INVINCIBILITY : GRAVITY_SWAP;
    items.emplace_back(900, randomZ, 20, randomType);
}

void Item::updateAll(Character* character) {
    for (auto& item : items) {
        item.update();
        item.checkCollision(character);
    }

    items.erase(std::remove_if(items.begin(), items.end(),
                               [](Item& item) { return item.isOutOfScreen(); }),
                                items.end());
}

void Item::renderAll(SDL_Renderer* renderer) {
    for (auto& item : items) {
        item.render(renderer);
    }
}
