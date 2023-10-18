#ifdef _WIN32
#include <windows.h>
#else

#endif
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "../headers/bot.h"
#include "../headers/ship.h"
#include "../headers/player.h"
#include "../headers/game.h"
#include "../headers/other.h"

extern int playerStep;

bool currentlySearching = true;
bool currentlySearchingVectorOfShip = false;
bool currentlySearchingByVectorTiles = false;

bool vector_direction = false;

int x, y;
char vector;
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 9);
std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 4);

bool Bot::makeShot(int x, int y)
{
    if (x >= 0 && x < 10 && y >= 0 && y < 10)
    {
        if (Player::getPrivateMatrix()[y][x] == 2)
        {
            Player::getPublicMatrix()[y][x] = 1;
            Player::getPrivateMatrix()[y][x] = 1;
            return true;
        }
        else
        {
            Player::getPublicMatrix()[y][x] = -1;
            return false;
        }
    }
    else
    {
        std::cout << "Invalid coordinates. Shot cannot be made." << std::endl;
        return false;
    }
}

bool Bot::shootOneOfFourClosesTile()
{
    if (y > 0 && Player::getPublicMatrix()[y - 1][x] == 0)
    {
        std::cout << "BOT: SEARCHING VECTOR: SHOOTING AT top" << std::endl;
        if (makeShot(x, y - 1))
        {
            vector = 'v';
            return true;
        }
        return false;
    }
    else if (x < 10 && Player::getPublicMatrix()[y][x + 1] == 0)
    {
        std::cout << "BOT: SEARCHING VECTOR: SHOOTING AT right" << std::endl;
        if (makeShot(x + 1, y))
        {
            vector = 'h';
            return true;
        }
        return false;
    }
    else if (y < 10 && Player::getPublicMatrix()[y + 1][x] == 0)
    {
        std::cout << "BOT: SEARCHING VECTOR: SHOOTING AT bottom" << std::endl;
        if (Bot::makeShot(x, y + 1))
        {
            vector = 'v';
            return true;
        }
        return false;
    }
    else if (x > 0 && Player::getPublicMatrix()[y][x - 1] == 0)
    {
        std::cout << "BOT: SEARCHING VECTOR: SHOOTING AT left" << std::endl;
        if (Bot::makeShot(x - 1, y))
        {
            vector = 'h';
            return true;
        }
        return false;
    }

    // that means that ship is single point
    std::cout << "BOT: SEARCHING VECTOR: END CAUSE ONE TILE SHIP" << std::endl;
    currentlySearching = true;
    currentlySearchingVectorOfShip = false;
    entry_point();
    return false;
}

void movingByYAxis()
{
    int v_y = y;
    while (!vector_direction)
    {
        v_y--;
        std::cout << "BOT: MOVING BY Y AXIS: MOVING UP" << std::endl;
        if (v_y >= 0)
        {
            if (Player::getPublicMatrix()[v_y][x] == 0) {
                if (!Bot::makeShot(x, v_y))
                {
                    std::cout << "BOT: MOVING BY Y AXIS: MOVING UP: MISSHOT" << std::endl;
                    playerStep = true;
                    vector_direction = true;
                    return;
                }
                std::cout << "BOT: MOVING BY Y AXIS: MOVING UP: SUCCESSFULL SHOT" << std::endl;
                playerStep = false;
            } else {
                std::cout << "BOT: MOVING BY Y AXIS: MOVING UP: SHOT HERE HAS BEEN ALREADY DONE" << std::endl;
            }
        }
        else
        {
            playerStep = true;
            vector_direction = true;
            return;
        }
    }

    v_y = y;
    while (true)
    {
        std::cout << "BOT: MOVING BY Y AXIS: MOVING BOTTOM" << std::endl;

        v_y++;

        if (v_y < 10)
        {
            if (Player::getPublicMatrix()[v_y][x] == 0) {
                if (!Bot::makeShot(x, v_y))
                {
                    std::cout << "BOT: MOVING BY Y AXIS: MOVING BOTTOM: MISSSHOT" << std::endl;

                    playerStep = true;
                    vector_direction = false;
                    currentlySearching = true;
                    currentlySearchingByVectorTiles = false;
                    return;
                }

                playerStep = false;
                std::cout << "BOT: MOVING BY Y AXIS: MOVING UP: SHOT SUCCESSFULL" << std::endl;
            } else {
                std::cout << "BOT: MOVING BY Y AXIS: MOVING UP: EDGE" << std::endl;
            }
        }
        else
        {
            playerStep = true;
            currentlySearching = true;
            currentlySearchingByVectorTiles = false;
            vector_direction = false;
            return;
        }
    }
}

void movingByXAxis()
{
    // moving left
    int v_x = x;
    while (!vector_direction)
    {
        std::cout << "BOT: MOVING BY AXIS: MOVING left" << std::endl;
        v_x--;
        if (v_x >= 0)
        {
            if (Player::getPublicMatrix()[y][v_x] == 0)
            {
                if (!Bot::makeShot(v_x, y))
                {
                    std::cout << "BOT: MOVING BY AXIS: MOVING left: MISSHOT" << std::endl;
                    playerStep = true;
                    vector_direction = true;
                    return;
                }

                playerStep = false;
                std::cout << "BOT: MOVING BY AXIS: MOVING left: SHOT SUCCESSFULL" << std::endl;
            } else {
                std::cout << "BOT: MOVING BY AXIS: MOVING left: SHOOT HERE HAS BEEN aLREADY DONE" << std::endl;
            }
        }
        else
        {
            std::cout << "BOT: MOVING BY AXIS: MOVING left: EDGE" << std::endl;
            playerStep = false;
            vector_direction = true;
            break;
        }
    }

    v_x = x;
    while (true)
    {
        std::cout << "BOT: MOVING BY AXIS: MOVING right" << std::endl;

        v_x++;
        if (v_x < 10)
        {
            if (Player::getPublicMatrix()[y][v_x] == 0 ) {

                if (!Bot::makeShot(v_x, y))
                {
                    std::cout << "BOT: MOVING BY AXIS: MOVING right: MISSHOT" << std::endl;
                    playerStep = true;
                    vector_direction = false;
                    currentlySearching = true;
                    currentlySearchingByVectorTiles = false;
                    return;
                }
                playerStep = false;
                std::cout << "BOT: MOVING BY AXIS: MOVING right: SHOT SUCCESSFULL" << std::endl;
            } else {
                std::cout << "BOT: MOVING BY AXIS: MOVING right: SHOOT HERE HAS BEEN aLREADY DONE" << std::endl;
            }
        }
        else
        {
            std::cout << "BOT: MOVING BY AXIS: MOVING right: EDGE" << std::endl;
            playerStep = true;
            currentlySearching = true;
            currentlySearchingByVectorTiles = false;
            vector_direction = false;
            return;
        }
    }
}

void Bot::entry_point()
{
    if (playerStep)
    {
        return;
    }

    if (currentlySearching)
    {
        std::cout << "BOT: Entry" << std::endl;
        if (Game::isEndOfGame()){
            #ifdef _WIN32
			MessageBoxA(NULL, Game::getWinner() == 'y' ? "You are winner" : "Enemy wins", "Game ends", MB_OK | MB_ICONINFORMATION);
            #endif
			Sleep(5);
			exit(1);
		}

        x = dist10(rng);
        y = dist10(rng);

        while (Player::getPublicMatrix()[y][x] != 0)
        {
            x = dist10(rng);
            y = dist10(rng);
        }
        std::cout << "BOT: INIT COORDS: " << x << " " << y << std::endl;

        if (Bot::makeShot(x, y))
        {
            currentlySearching = false;
            currentlySearchingVectorOfShip = true;
            std::cout << "BOT: INIT COORDS: SUCCESS" << std::endl;
        }
        else
        {
            playerStep = true;
            currentlySearching = true;
            currentlySearchingVectorOfShip = false;
            std::cout << "BOT: INIT COORDS: MISSSHOT" << std::endl;
            return;
        }
    }

    if (currentlySearchingVectorOfShip)
    {
        std::cout << "BOT: SEARCHING VECTOR OF SHIP" << std::endl;

        if (shootOneOfFourClosesTile())
        {
            std::cout << "BOT: SEARCHING VECTOR OF SHIP: VECTOR FOUND" << std::endl;
            currentlySearchingVectorOfShip = false;
            currentlySearchingByVectorTiles = true;
            playerStep = false;
        }
        else
        {
            std::cout << "BOT: SEARCHING VECTOR OF SHIP: VECTOR NOT FOUND" << std::endl;
            playerStep = true;
            return;
        }
    }

    if (currentlySearchingByVectorTiles)
    {
        if (vector == 'h')
        {
            std::cout << "Shoot vector tiles by X" << std::endl;

            movingByXAxis();
        }
        else
        {
            std::cout << "Shoot vector tiles by Y" << std::endl;

            movingByYAxis();
        }
    }
}

void Bot::placeShip(int size, int orientation, int x, int y)
{
    if (orientation == 0)
    { // 'h' for horizontal, 'v' for vertical
        for (int i = 0; i < size; i++)
        {
            Bot::getPrivateMatrix()[y][x + i] = 2;
        }
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            Bot::getPrivateMatrix()[y + i][x] = 2;
        }
    }
}

bool Bot::hasMinimumDistance(int x, int y, int size, int orientation)
{
    if (orientation == 0)
    { // Horizontal placement
        for (int i = y - 1; i < y + size + 1; i++)
        {
            for (int j = x - 1; j < x + size + 1; j++)
            {
                if (i >= 0 && i < 10 && j >= 0 && j < 10)
                {
                    if (Bot::getPrivateMatrix()[i][j] == 2)
                    {
                        return false;
                    }
                }
            }
        }
    }
    else
    { // Vertical placement
        for (int i = y - 1; i < y + size + 1; i++)
        {
            for (int j = x - 1; j < x + 2; j++)
            {
                if (i >= 0 && i < 10 && j >= 0 && j < 10)
                {
                    if (Bot::getPrivateMatrix()[i][j] == 2)
                    {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Bot::generateShipsOnMatrix()
{
    // place this ships to SHIP CLASS

    std::random_device rd;
    std::mt19937 rng(rd());

    for (const ShipType &st : ShipHandler::getShipTypes())
    {
        for (int i = 0; i < st.number; i++)
        {
            int x, y, size, orientation;
            bool validPlacement = false;

            while (!validPlacement)
            {
                x = std::uniform_int_distribution<int>(0, 10 - 1)(rng);
                y = std::uniform_int_distribution<int>(0, 10 - 1)(rng);
                size = st.size;
                orientation = std::uniform_int_distribution<int>(0, 1)(rng); // Randomly choose 'h' or 'v'

                if (orientation == 0)
                { // Horizontal placement
                    if (x + size > 10)
                    {
                        continue;
                    }
                }
                else
                { // Vertical placement
                    if (y + size > 10)
                    {
                        continue;
                    }
                }

                if (hasMinimumDistance(x, y, size, orientation))
                {
                    validPlacement = true;
                    placeShip(size, orientation, x, y);
                }
            }
        }
    }
}