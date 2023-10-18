#pragma once

#include "game.h"

class Enemy {
public:
    static inline int (*getPrivateMatrix())[Game::matrixS]
    {
        static int enemy_warship_matrix_private[Game::matrixS][Game::matrixS];
        return enemy_warship_matrix_private;
    }

    static inline int (*getPublicMatrix())[Game::matrixS]
    {
        static int enemy_warship_matrix_public[Game::matrixS][Game::matrixS];
        return enemy_warship_matrix_public;
    };
};