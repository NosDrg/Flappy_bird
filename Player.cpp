#include "Player.h"

void Player::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void Player::move() {
    x += dirX * TILE_SIZE;
    y += dirY * TILE_SIZE;
    rect.x = x;
    rect.y = y;
}
