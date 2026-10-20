#pragma once

class Enemy
{
public:
    Enemy(int startX, int startY, int health = 15, int damage = 5, char symbol = 'G');

    int getX() const;
    int getY() const;
    int getHealth() const;
    int getDamage() const;
    char getSymbol() const;

    void takeDamage(int amount);
    bool isAlive() const;

private:
    int x;
    int y;
    int health;
    int damage;
    char symbol;
};
