#include "Enemy.h"

Enemy::Enemy(int startX, int startY, int health, int damage, char symbol)
    : x(startX), y(startY), health(health), damage(damage), symbol(symbol)
{
}

int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
int Enemy::getHealth() const { return health; }
int Enemy::getDamage() const { return damage; }
char Enemy::getSymbol() const { return symbol; }

void Enemy::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
        health = 0;
}

bool Enemy::isAlive() const
{
    return health > 0;
}
