#include "Game.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

Game::Game() : grid(std::make_unique<Grid>(15, 9)), player(START_X, START_Y)
{
}

void Game::run()
{
    bool running = true;
    while (running)
    {
        showMenu();
        int choice = readMenuChoice(1, 3);

        switch (choice)
        {
        case 1: play(); break;
        case 2: showInstructions(); break;
        case 3: running = false; break;
        }
    }
    std::cout << "\nThanks for playing!\n";
}

void Game::showMenu() const
{
    std::cout << "\n=== DUNGEON CRAWLER ===\n"
        << "1. Play\n"
        << "2. Instructions\n"
        << "3. Quit\n";
}

void Game::showInstructions() const
{
    std::cout << "\n--- Instructions ---\n"
        << "Explore the dungeon, collect items, and reach the exit.\n"
        << "Move with W (up), A (left), S (down), D (right), then press Enter.\n"
        << "Type Q to return to the menu.\n\n"
        << "@ = you   # = wall   . = floor   E = exit\n"
        << "$ = treasure (adds to score)   ! = potion (goes in your inventory)\n";
}

void Game::play()
{
    grid = std::make_unique<Grid>(15, 9);   // New dungeon for every run
    grid->buildTestRoom();
    spawnEnemies();
    player.reset(START_X, START_Y);
    message = "You enter the dungeon.";

    bool playing = true;
    while (playing)
    {
        draw();
        std::cout << "Move (W/A/S/D) or Q to quit: ";
        std::string input = readLine();

        // Only accept exactly one character.
        if (input.size() != 1)
        {
            message = "Invalid command. Use W, A, S, D, or Q.";
            continue;
        }

        char command = static_cast<char>(std::toupper(static_cast<unsigned char>(input[0])));

        bool moved = false;
        switch (command)
        {
        case 'W': moved = tryMove(0, -1); break;
        case 'S': moved = tryMove(0, 1);  break;
        case 'A': moved = tryMove(-1, 0); break;
        case 'D': moved = tryMove(1, 0);  break;
        case 'Q': playing = false; continue;
        default:
            message = "Invalid command. Use W, A, S, D, or Q.";
            continue;
        }

        if (moved && handleTile())
        {
            draw();
            std::cout << "\n*** YOU ESCAPED THE DUNGEON! ***\n"
                << "Score: " << player.getScore()
                << "  Potions: " << player.getPotions()
                << "  Steps: " << player.getSteps() << "\n";
            playing = false;
        }
    }
}

bool Game::tryMove(int dx, int dy)
{
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;

    if (!grid->isWalkable(newX, newY))
    {
        message = "A wall blocks your way.";
        return false;
    }

    // An enemy blocks the tile: attack it instead of moving.
    int enemyIndex = findEnemyIndex(newX, newY);
    if (enemyIndex != -1)
    {
        fight(enemyIndex);
        return false;   // The player stays where they are
    }

    player.moveTo(newX, newY);
    message = "";
    return true;
}

void Game::fight(int enemyIndex)
{
    Enemy& enemy = enemies[enemyIndex];

    int playerDamage = player.getAttack();
    enemy.takeDamage(playerDamage);

    if (!enemy.isAlive())
    {
        message = "You defeated the goblin!";
        enemies.erase(enemies.begin() + enemyIndex);
        return;
    }

    int enemyDamage = enemy.getDamage();
    player.takeDamage(enemyDamage);
    message = "You hit the goblin for " + std::to_string(playerDamage)
        + " (goblin HP: " + std::to_string(enemy.getHealth())
        + "). It hits you for " + std::to_string(enemyDamage) + ".";
}

bool Game::handleTile()
{
    // Pointer to the tile under the player, so we can change it in place.
    Tile* tile = grid->getTilePtr(player.getX(), player.getY());
    if (tile == nullptr)
        return false;   // Should never happen, but never dereference nullptr

    switch (*tile)
    {
    case Tile::Treasure:
        player.addScore(10);
        *tile = Tile::Floor;   // Item is gone once collected
        message = "You found treasure! +10 score.";
        break;
    case Tile::Potion:
        player.addPotion();
        *tile = Tile::Floor;
        message = "You picked up a potion.";
        break;
    case Tile::Exit:
        return true;
    default:
        break;
    }
    return false;
}

void Game::spawnEnemies()
{
    enemies.clear();
    enemies.push_back(Enemy(5, 4));
    enemies.push_back(Enemy(5, 6));
    enemies.push_back(Enemy(12, 4));
}

int Game::findEnemyIndex(int x, int y) const
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        if (enemies[i].getX() == x && enemies[i].getY() == y)
            return static_cast<int>(i);
    }
    return -1;
}

void Game::draw() const
{
    std::cout << "\n";
    for (int y = 0; y < grid->getHeight(); ++y)
    {
        for (int x = 0; x < grid->getWidth(); ++x)
        {
            if (x == player.getX() && y == player.getY())
            {
                std::cout << '@';
                continue;
            }
            int enemyIndex = findEnemyIndex(x, y);
            if (enemyIndex != -1)
            {
                std::cout << enemies[enemyIndex].getSymbol();
                continue;
            }
            switch (grid->getTile(x, y))
            {
            case Tile::Wall:     std::cout << '#'; break;
            case Tile::Exit:     std::cout << 'E'; break;
            case Tile::Treasure: std::cout << '$'; break;
            case Tile::Potion:   std::cout << '!'; break;
            case Tile::Floor:    std::cout << '.'; break;
            }
        }
        std::cout << "\n";
    }
    std::cout << "HP: " << player.getHealth()
        << "  Score: " << player.getScore()
        << "  Potions: " << player.getPotions()
        << "  Steps: " << player.getSteps() << "\n";
    if (!message.empty())
        std::cout << message << "\n";
}

std::string Game::readLine() const
{
    std::string line;
    if (!std::getline(std::cin, line))
    {
        // Input stream closed (e.g. Ctrl+Z / Ctrl+D): exit cleanly instead of looping forever.
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
    return line;
}

int Game::readMenuChoice(int min, int max) const
{
    while (true)
    {
        std::cout << "Choose (" << min << "-" << max << "): ";
        std::istringstream parser(readLine());

        int value;
        char extra;
        if ((parser >> value) && !(parser >> extra) && value >= min && value <= max)
            return value;

        std::cout << "Invalid choice. Please enter a number from "
            << min << " to " << max << ".\n";
    }
}
