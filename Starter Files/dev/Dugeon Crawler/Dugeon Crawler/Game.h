#pragma once
#include <string>
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

    Grid grid;
    Player player;
    std::string message;   // Feedback shown after each command
};
