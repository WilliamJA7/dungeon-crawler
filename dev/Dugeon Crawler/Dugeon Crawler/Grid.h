#pragma once
#include <vector>
#include "Tile.h"

// Stores the dungeon map in a vector of vectors: tiles[y][x]
class Grid
{
public:
    Grid(int width, int height);

    int getWidth() const;
    int getHeight() const;

    bool inBounds(int x, int y) const;
    Tile getTile(int x, int y) const;
    Tile* getTilePtr(int x, int y);
    void setTile(int x, int y, Tile tile);
    bool isWalkable(int x, int y) const;

    // Fills the grid with a walled room and an exit (hard-coded for Week 1).
    void buildTestRoom();

private:
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;
};
