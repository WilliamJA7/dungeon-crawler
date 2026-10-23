#pragma once

class Player
{
public:
    static const int MAX_HEALTH = 20;

    Player(int startX = 1, int startY = 1);

    int getX() const;
    int getY() const;
    int getHealth() const;
    int getScore() const;
    int getPotions() const;
    int getSteps() const;
    int getAttack() const;
    bool isAlive() const;

    void setPosition(int newX, int newY);
    void moveTo(int newX, int newY);
    void addScore(int amount);
    void addPotion();
    void takeDamage(int amount);
    void heal(int amount);        // Never goes above MAX_HEALTH
    bool usePotion();             // Returns false if there are no potions

    // Puts the player back to a fresh state for a new run.
    void reset(int startX, int startY);

private:
    int x;
    int y;
    int health;
    int score;
    int potions;
    int steps;
    int attack;
};
