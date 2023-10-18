#pragma once

#include "enemy.h"

    // 1. We will take random point on matrix and hit it;
    // 2. if not ship - skip step and then restart bot;
    // 3. if hit - bot will make another step to next functionand will try to hit on of 4 random closest tile(if not on the edge)
    // 4. try to hit untill success -> if not then it was ship for 1 tile...etc restart bot
    // 5. if success -> we are getting vector of ship placement, and firstly hittin till on edge of ship, and then to another
    
class Bot : public Enemy
{
public:
    static void entry_point();

    static bool makeShot(int x, int y);

    static void generateShipsOnMatrix();

private:
    static bool shootOneOfFourClosesTile();

    static bool hasMinimumDistance(int x, int y, int size, int orientation);

    static void placeShip(int size, int orientation, int x, int y);
};