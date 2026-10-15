#include "Game.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>

Game::Game() : grid(15, 9), player(START_X, START_Y)
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
    grid.buildTestRoom();
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

    if (!grid.isWalkable(newX, newY))
    {
        message = "A wall blocks your way.";
        return false;
    }

    player.moveTo(newX, newY);
    message = "";
    return true;
}

bool Game::handleTile()
{
    int x = player.getX();
    int y = player.getY();

    switch (grid.getTile(x, y))
    {
    case Tile::Treasure:
        player.addScore(10);
        grid.setTile(x, y, Tile::Floor);   // Item is gone once collected
        message = "You found treasure! +10 score.";
        break;
    case Tile::Potion:
        player.addPotion();
        grid.setTile(x, y, Tile::Floor);
        message = "You picked up a potion.";
        break;
    case Tile::Exit:
        return true;
    default:
        break;
    }
    return false;
}

void Game::draw() const
{
    std::cout << "\n";
    for (int y = 0; y < grid.getHeight(); ++y)
    {
        for (int x = 0; x < grid.getWidth(); ++x)
        {
            if (x == player.getX() && y == player.getY())
            {
                std::cout << '@';
                continue;
            }
            switch (grid.getTile(x, y))
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