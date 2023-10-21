#pragma once

#include "game.h"
#include "conf.h"

class Enemy {
public:
    static inline int (*getPrivateMatrix())[Conf::matrixS]
    {
        static int enemy_warship_matrix_private[Conf::matrixS][Conf::matrixS];
        return enemy_warship_matrix_private;
    }

    static inline int (*getPublicMatrix())[Conf::matrixS]
    {
        static int enemy_warship_matrix_public[Conf::matrixS][Conf::matrixS];
        return enemy_warship_matrix_public;
    };
};