#include "Grid.h"

Grid::Grid(int width, int height)
    : width(width), height(height), tiles(height, std::vector<Tile>(width, Tile::Floor))
{
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }

bool Grid::inBounds(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

Tile Grid::getTile(int x, int y) const
{
    // Out-of-bounds counts as a wall so callers can't read invalid memory.
    if (!inBounds(x, y))
        return Tile::Wall;
    return tiles[y][x];
}

void Grid::setTile(int x, int y, Tile tile)
{
    if (inBounds(x, y))
        tiles[y][x] = tile;
}

bool Grid::isWalkable(int x, int y) const
{
    return getTile(x, y) != Tile::Wall;
}

void Grid::buildTestRoom()
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            bool onBorder = (x == 0 || y == 0 || x == width - 1 || y == height - 1);
            tiles[y][x] = onBorder ? Tile::Wall : Tile::Floor;
        }
    }

    // A few interior walls so the map isn't just an empty box.
    for (int y = 2; y < height - 3; ++y)
        setTile(width / 2, y, Tile::Wall);

    // Week 2: items to pick up.
    setTile(3, 3, Tile::Treasure);
    setTile(11, 3, Tile::Treasure);
    setTile(12, 5, Tile::Treasure);
    setTile(4, 6, Tile::Potion);
    setTile(10, 6, Tile::Potion);

    // Exit in the bottom-right corner of the room.
    setTile(width - 2, height - 2, Tile::Exit);
}