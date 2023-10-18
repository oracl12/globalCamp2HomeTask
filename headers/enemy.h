#pragma once

class Enemy {
public:
    static inline int (*getPrivateMatrix())[10]
    {
        static int enemy_warship_matrix_private[10][10];
        return enemy_warship_matrix_private;
    }

    static inline int (*getPublicMatrix())[10]
    {
        static int enemy_warship_matrix_public[10][10];
        return enemy_warship_matrix_public;
    };
};