#include "../headers/player.h"
#include "../headers/bot.h"
#include "../headers/ship.h"

#include <iostream>

extern int start_pos_x;
extern int start_pos_y;

extern int size;

bool Player::makeShot(int x, int y)
{
    if (x >= 0 && x < 10 && y >= 0 && y < 10)
    {
        if (Enemy::getPrivateMatrix()[y][x] == 2)
        {
            // if successful mark it as 1 in the public
            Enemy::getPublicMatrix()[y][x] = 1;
            Enemy::getPrivateMatrix()[y][x] = 1;
            std::cout << "Player: Shot successful!" << std::endl;
            return true;
        }
        else
        {
            if (Enemy::getPrivateMatrix()[y][x] != 1) {
                Enemy::getPublicMatrix()[y][x] = -1;
            }
            std::cout << "Player: Shot missed!" << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Player: Invalid coordinates. Shot cannot be made." << std::endl;
        return false;
    }
}

void Player::changeShipPosition(int mouseX, int mouseY, Ship* ship){
    int counter = 0;
    std::cout << "Player: changing position" << std::endl;
    if (ship->relative_position == 'h') {
        for (Rect& rect : ship->rects) {
            rect.x = mouseX - 20 + counter * size;
            rect.y = mouseY - 20;
            counter++;
        }
    } else if (ship->relative_position == 'v') {
        for (Rect& rect : ship->rects) {
            rect.x = mouseX - 20;
            rect.y = mouseY - 20 + counter * size;
            counter++;
        }
    }
}

bool Player::placeShip(int startX, int startY, Ship& ship) {
	int shipLength = ship.rects.size();
    int counter = 0;

    if (ShipHandler::canPlaceShip(startX, startY, ship.relative_position, shipLength)) {
        if (ship.relative_position == 'h') {
            for (int x = startX; x < startX + shipLength; ++x) {
                getPrivateMatrix()[startY][x] = 2;
                ship.rects.at(counter).placeOnMatrix = { x, startY };
                counter++;
            }
        } else if (ship.relative_position == 'v') {
            for (int y = startY; y < startY + shipLength; ++y) {
                getPrivateMatrix()[y][startX] = 2;
                ship.rects.at(counter).placeOnMatrix = { startX, y };
                counter++;
            }
        }
        return true; // Ship placed successfully
    }
    return false; // Ship placement failed
}

bool Player::removeShip(int startX, int startY, Ship& ship) {
    if (getPrivateMatrix()[startY][startX] != 2){
        return false;
    }
    
	int shipLength = ship.rects.size();
    if (ship.relative_position == 'h') {
        for (int x = ship.rects[0].placeOnMatrix.x; x < ship.rects[0].placeOnMatrix.x + shipLength; ++x) {
            getPrivateMatrix()[startY][x] = 0; // Clear the cell
        }
    } else if (ship.relative_position == 'v') {
        for (int y = ship.rects[0].placeOnMatrix.y; y < ship.rects[0].placeOnMatrix.y + shipLength; ++y) {
            getPrivateMatrix()[y][startX] = 0; // Clear the cell
        }
    }
    ship.avaible = true;
    return true;
}

void Player::changeDirection(Ship& ship, char direction) {
    if (direction == ship.relative_position) {
        return;
    }

    if (ship.relative_position == 'h') {
        // Convert from horizontal to vertical
        int currentY = ship.rects[0].y;
        for (int i = 0; i < ship.rects.size(); ++i) {
            ship.rects[i].x = ship.rects[0].x;
            ship.rects[i].y = currentY;
            currentY += ship.rects[i].height;
        }

        ship.relative_position = 'v';
    } else if (ship.relative_position == 'v') {
        // Convert from vertical to horizontal
        int currentX = ship.rects[0].x;
        for (int i = 0; i < ship.rects.size(); ++i) {
            ship.rects[i].x = currentX;
            ship.rects[i].y = ship.rects[0].y;
            currentX += ship.rects[i].width;
        }

        ship.relative_position = 'h';
    }
}