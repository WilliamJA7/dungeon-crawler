#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Enemy.h"
#include "Grid.h"
#include "Player.h"

class Game
{
public:
    Game();
    void run();   // Main menu loop

private:
    void showMenu() const;
    void showInstructions() const;
    void play();  // Core game loop for one dungeon run
    void draw() const;

    // Week 3: enemies
    void spawnEnemies();
    // Returns the index of the enemy at (x, y), or -1 if there is none.
    int findEnemyIndex(int x, int y) const;
    // One round of combat: the player hits, then the enemy hits back if it survives.
    void fight(int enemyIndex);

    // Tries to move the player by (dx, dy). Returns true if the move happened.
    bool tryMove(int dx, int dy);
    // Handles whatever is on the tile the player just stepped onto.
    // Returns true if the player reached the exit (win condition).
    bool handleTile();

    // Reads a whole line and only returns when it is a number in [min, max].
    int readMenuChoice(int min, int max) const;
    std::string readLine() const;

    static const int START_X = 1;
    static const int START_Y = 1;

    std::unique_ptr<Grid> grid;   // Owned pointer: a fresh dungeon is created each run
    Player player;
    std::vector<Enemy> enemies;
    std::string message;   // Feedback shown after each command
};
