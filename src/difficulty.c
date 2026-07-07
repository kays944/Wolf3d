/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** difficulty.c — easy/normal/nightmare multipliers
*/

#include "macros.h"
#include "proto.h"

float diff_hp_mult(int d)
{
    static const float mult[DIFF_COUNT] = {0.7f, 1.0f, 1.4f};

    if (d < 0 || d >= DIFF_COUNT)
        return 1.0f;
    return mult[d];
}

float diff_dmg_mult(int d)
{
    static const float mult[DIFF_COUNT] = {0.6f, 1.0f, 1.5f};

    if (d < 0 || d >= DIFF_COUNT)
        return 1.0f;
    return mult[d];
}

float diff_night_mult(int d)
{
    static const float mult[DIFF_COUNT] = {0.7f, 1.0f, 1.3f};

    if (d < 0 || d >= DIFF_COUNT)
        return 1.0f;
    return mult[d];
}

const char *diff_label(int d)
{
    if (d == DIFF_EASY)
        return "< FACILE >";
    if (d == DIFF_HARD)
        return "< CAUCHEMAR >";
    return "< NORMAL >";
}
