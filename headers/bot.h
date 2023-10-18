#pragma once

#include "enemy.h"

class Bot : public Enemy
{
public:
    static void entry_point();

    static bool makeShot(int x, int y);

    static void generateShipsOnMatrix();

private:
    // firslty we will take random point on matrix and hit it;
    // if not ship - skip step and then restart func;
    // if hit - bot will make another step from next function(if ship bigger than 1>) and will try to hit on of 4 random closest tile(if not on the edge)
    // if dont hit ship
    // void makeRandomMove();

    static bool shootOneOfFourClosesTile();

    static bool hasMinimumDistance(int x, int y, int size, int orientation);

    static void placeShip(int size, int orientation, int x, int y);
};