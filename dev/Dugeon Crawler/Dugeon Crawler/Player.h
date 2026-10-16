#pragma once

class Player
{
public:
    Player(int startX = 1, int startY = 1)
        : x(startX), y(startY), health(20), score(0), potions(0), steps(0) {
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getHealth() const { return health; }
    int getScore() const { return score; }
    int getPotions() const { return potions; }
    int getSteps() const { return steps; }

    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void moveTo(int newX, int newY) { x = newX; y = newY; ++steps; }
    void addScore(int amount) { score += amount; }
    void addPotion() { ++potions; }

    // Puts the player back to a fresh state for a new run.
    void reset(int startX, int startY)
    {
        x = startX; y = startY;
        health = 20; score = 0; potions = 0; steps = 0;
    }

private:
    int x;
    int y;
    int health;
    int score;
    int potions;
    int steps;
};
