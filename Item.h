#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Game.h"
#include "Character.h"
#include <vector>

class Item {
public:
    enum ItemType { HEALTH, INVINCIBILITY, GRAVITY_SWAP }; // Thêm loại mới

    Item(int x, int z, int size, ItemType type);

    void update();
    void render(SDL_Renderer* renderer);
    bool checkCollision(Character* character);
    bool isOutOfScreen();

    static std::vector<Item> items;
    static void spawnRandomItem();
    static void updateAll(Character* character);
    static void renderAll(SDL_Renderer* renderer);

private:
    int x, z, size;
    ItemType type;
    bool active;
};

#include "Item.cpp"

#endif // ITEM_H_INCLUDED
