#include "Game.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace
{
    const std::string INVALID_COMMAND = "Invalid command. Use W, A, S, D, H, or Q.";
    const int MAP_WIDTH = 15;
    const int MAP_HEIGHT = 9;
    const int TREASURE_VALUE = 10;
}

Game::Game() : grid(std::make_unique<Grid>(MAP_WIDTH, MAP_HEIGHT)), player(START_X, START_Y)
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
        << "Explore the dungeon, defeat goblins, and reach the exit.\n"
        << "Move with W (up), A (left), S (down), D (right), then press Enter.\n"
        << "Walk into a goblin to attack it. Goblins hit back!\n"
        << "Type H to drink a potion and heal. Type Q to return to the menu.\n\n"
        << "@ = you   # = wall   . = floor   E = exit   G = goblin\n"
        << "$ = treasure (adds to score)   ! = potion (goes in your inventory)\n";
}

void Game::play()
{
    grid = std::make_unique<Grid>(MAP_WIDTH, MAP_HEIGHT);   // New dungeon for every run
    grid->buildTestRoom();
    spawnEnemies();
    player.reset(START_X, START_Y);
    message = "You enter the dungeon.";

    while (true)
    {
        draw();
        std::cout << "Move (W/A/S/D), H to heal, Q to quit: ";
        std::string input = readLine();

        // Only accept exactly one character.
        if (input.size() != 1)
        {
            message = INVALID_COMMAND;
            continue;
        }

        char command = static_cast<char>(std::toupper(static_cast<unsigned char>(input[0])));
        if (command == 'Q')
            return;

        if (!handleCommand(command))
        {
            message = INVALID_COMMAND;
            continue;
        }

        if (checkEndConditions())
            return;
    }
}

bool Game::handleCommand(char command)
{
    switch (command)
    {
    case 'W': move(0, -1); return true;
    case 'S': move(0, 1);  return true;
    case 'A': move(-1, 0); return true;
    case 'D': move(1, 0);  return true;
    case 'H': usePotion(); return true;
    default:  return false;
    }
}

void Game::move(int dx, int dy)
{
    if (tryMove(dx, dy))
        handleTile();
}

bool Game::checkEndConditions()
{
    if (grid->getTile(player.getX(), player.getY()) == Tile::Exit)
    {
        draw();
        std::cout << "\n*** YOU ESCAPED THE DUNGEON! ***\n"
            << "Score: " << player.getScore()
            << "  Potions: " << player.getPotions()
            << "  Steps: " << player.getSteps() << "\n";
        return true;
    }

    if (!player.isAlive())
    {
        draw();
        std::cout << "\n*** YOU DIED. GAME OVER. ***\n"
            << "Score: " << player.getScore()
            << "  Steps: " << player.getSteps() << "\n";
        return true;
    }

    return false;
}

void Game::usePotion()
{
    if (player.getPotions() <= 0)
    {
        message = "You have no potions.";
        return;
    }
    if (player.getHealth() >= Player::MAX_HEALTH)
    {
        message = "You are already at full health.";
        return;
    }

    int healthBefore = player.getHealth();
    player.usePotion();
    player.heal(POTION_HEAL);
    message = "You drink a potion and heal "
        + std::to_string(player.getHealth() - healthBefore) + " HP.";
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
    Enemy* enemy = findEnemyAt(newX, newY);
    if (enemy != nullptr)
    {
        fight(*enemy);
        removeDefeatedEnemies();   // Safe now: we are done using the pointer
        return false;              // The player stays where they are
    }

    player.moveTo(newX, newY);
    message = "";
    return true;
}

void Game::fight(Enemy& enemy)
{
    int playerDamage = player.getAttack();
    enemy.takeDamage(playerDamage);

    if (!enemy.isAlive())
    {
        message = "You defeated the goblin!";
        return;
    }

    int enemyDamage = enemy.getDamage();
    player.takeDamage(enemyDamage);
    message = "You hit the goblin for " + std::to_string(playerDamage)
        + " (goblin HP: " + std::to_string(enemy.getHealth())
        + "). It hits you for " + std::to_string(enemyDamage) + ".";
}

void Game::removeDefeatedEnemies()
{
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return !e.isAlive(); }),
        enemies.end());
}

void Game::handleTile()
{
    // Pointer to the tile under the player, so we can change it in place.
    Tile* tile = grid->getTilePtr(player.getX(), player.getY());
    if (tile == nullptr)
        return;   // Should never happen, but never dereference nullptr

    switch (*tile)
    {
    case Tile::Treasure:
        player.addScore(TREASURE_VALUE);
        *tile = Tile::Floor;   // Item is gone once collected
        message = "You found treasure! +10 score.";
        break;
    case Tile::Potion:
        player.addPotion();
        *tile = Tile::Floor;
        message = "You picked up a potion.";
        break;
    default:
        break;
    }
}

void Game::spawnEnemies()
{
    enemies.clear();
    enemies.push_back(Enemy(5, 4));
    enemies.push_back(Enemy(5, 6));
    enemies.push_back(Enemy(12, 4));
}

Enemy* Game::findEnemyAt(int x, int y)
{
    for (Enemy& enemy : enemies)
    {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y)
            return &enemy;
    }
    return nullptr;
}

const Enemy* Game::findEnemyAt(int x, int y) const
{
    for (const Enemy& enemy : enemies)
    {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y)
            return &enemy;
    }
    return nullptr;
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
            const Enemy* enemy = findEnemyAt(x, y);
            if (enemy != nullptr)
            {
                std::cout << enemy->getSymbol();
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