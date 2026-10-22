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
    bool handleCommand(char command);   // Returns false if the command is invalid
    void move(int dx, int dy);          // Moves, then handles whatever is on the new tile
    bool checkEndConditions();          // Returns true if the run is over (win or loss)
    void draw() const;

    // Week 3: enemies
    void spawnEnemies();
    // Returns a pointer to the living enemy at (x, y), or nullptr if there is none.
    Enemy* findEnemyAt(int x, int y);
    const Enemy* findEnemyAt(int x, int y) const;   // Version used by draw()
    // One round of combat: the player hits, then the enemy hits back if it survives.
    void fight(Enemy& enemy);
    // Erases defeated enemies from the vector.
    void removeDefeatedEnemies();
    // Drinks a potion to heal, with a message if that isn't possible.
    void usePotion();

    // Tries to move the player by (dx, dy). Returns true if the move happened.
    bool tryMove(int dx, int dy);
    // Handles whatever is on the tile the player just stepped onto.
    void handleTile();

    // Reads a whole line and only returns when it is a number in [min, max].
    int readMenuChoice(int min, int max) const;
    std::string readLine() const;

    static const int START_X = 1;
    static const int START_Y = 1;
    static const int POTION_HEAL = 8;

    std::unique_ptr<Grid> grid;   // Owned pointer: a fresh dungeon is created each run
    Player player;
    std::vector<Enemy> enemies;
    std::string message;   // Feedback shown after each command
};