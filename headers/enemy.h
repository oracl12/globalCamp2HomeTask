#pragma once

#include "game.h"
#include "conf.h"

class Enemy {
public:
    static inline Conf::TileStatus (*getPrivateMatrix())[Conf::matrixS]
    {
        static Conf::TileStatus enemy_warship_matrix_private[Conf::matrixS][Conf::matrixS];
        return enemy_warship_matrix_private;
    }

    static inline Conf::TileStatus (*getPublicMatrix())[Conf::matrixS]
    {
        static Conf::TileStatus enemy_warship_matrix_public[Conf::matrixS][Conf::matrixS];
        return enemy_warship_matrix_public;
    };
};