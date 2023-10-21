#pragma once

#include "ship.h"


class Player
{
public:
    static inline int (*getPrivateMatrix())[10] // reokace 10 with matrix size
    {
        static int player_warship_matrix_private[10][10];
        return player_warship_matrix_private;
    };

    static inline int (*getPublicMatrix())[10]
    {
        static int player_warship_matrix_public[10][10];
        return player_warship_matrix_public;
    };

    static bool makeShot(int x, int y);
    static void changeDirection(Ship &ship, char direction);
    static bool placeShip(int startX, int startY, Ship &ship);
    static bool removeShip(int startX, int startY, Ship &ship);
    static void changeShipPosition(int mouseX, int mouseY, Ship *ship);
};