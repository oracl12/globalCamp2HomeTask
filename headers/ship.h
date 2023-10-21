#pragma once
#include "renderer.h"

#include <iostream>
#include <vector>
#include <string>

struct Ship
{
    std::string id; // to remove
    std::vector<Rect> rects; // array of rectangles for this ship
    char relative_position = 'h'; // replace with enum // h - horizontal, v - vertical
    bool avaible = true;
};

struct ShipType
{
    int size;
    int number;
};

class ShipHandler
{
public:
    friend class Player;

    static std::vector<Ship> &getShips(){
        static std::vector<Ship> ships;
        return ships;
    };

    static Ship* findShipByMouse(int mouseX, int mouseY);
    static int getShipLengthById(const Ship& ship);
    static void fillUpShips();
    static bool isDestroyed(Ship* ship);
    static Ship* getShipFromMatrix(int x, int y);
    static std::vector<ShipType> getShipTypes();
private:
    static bool IsMouseInRect(int mouseX, int mouseY, const Rect& rect);
    static bool canPlaceShip(int startX, int startY, char orientation, int shipLength);
};