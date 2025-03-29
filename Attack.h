#ifndef ATTACK_H_INCLUDED
#define ATTACK_H_INCLUDED

#include "Game.h"
#include "Character.h"
#include <vector>

class Attack {
public:
    enum AttackType { PROJECTILE, LASER };
    enum LaserState { CHARGING, FIRING };

    Attack(int x, int z, int width, int height, int speed, int damage, AttackType type);

    void update(int score);
    void render(SDL_Renderer* renderer);
    bool checkCollision(Character* character);
    bool isOutOfScreen();

    static std::vector<Attack> attacks;
    static void spawnRandomAttack();
    static void spawnLaser();
    static void updateAll(Character* character);
    static void renderAll(SDL_Renderer* renderer);

private:
    int x, z, width, height;
    int speed;
    int damage;
    bool active;
    AttackType type;
    int speedBullet;

    LaserState laserState;
    int chargeTime;
    int fireTime;
    int timer;
};

#include "Attack.cpp"

#endif // ATTACK_H_INCLUDED
