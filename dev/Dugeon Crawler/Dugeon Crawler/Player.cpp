#include "Player.h"

Player::Player(int startX, int startY)
    : x(startX), y(startY), health(MAX_HEALTH), score(0), potions(0), steps(0), attack(5)
{
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }
int Player::getHealth() const { return health; }
int Player::getScore() const { return score; }
int Player::getPotions() const { return potions; }
int Player::getSteps() const { return steps; }
int Player::getAttack() const { return attack; }
bool Player::isAlive() const { return health > 0; }

void Player::setPosition(int newX, int newY)
{
    x = newX;
    y = newY;
}

void Player::moveTo(int newX, int newY)
{
    x = newX;
    y = newY;
    ++steps;
}

void Player::addScore(int amount) { score += amount; }
void Player::addPotion() { ++potions; }

void Player::takeDamage(int amount)
{
    health -= amount;
    if (health < 0)
        health = 0;
}

void Player::reset(int startX, int startY)
{
    x = startX;
    y = startY;
    health = MAX_HEALTH;
    score = 0;
    potions = 0;
    steps = 0;
}
