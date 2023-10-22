#pragma once

#include "string"

class Conf
{
public:
    enum class GameMode {
        BOT,
        NETWORK
    };

    enum class TileStatus {
        CLEARED = -1,
        HIDDEN = 0,
        DESTROYED = 1,
        SHIP = 2 
    };


    static int port;
    static GameMode gameMode;
    static bool host;
    static bool debug;

    static const int size = 50;
    static const int start_l_x = 75;
    static const int start_r_x = 740 + 6;
    static const int matrixS = 10;
    static std::wstring windowTitle;
};