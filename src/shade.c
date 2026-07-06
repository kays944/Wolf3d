/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** shade.c -- distance-based darkening, stronger at night
*/

#include "macros.h"
#include "proto.h"

float world_shade(float dist, map_t *m, player_t *p)
{
    float sight = 0;
    float b = 0;

    if (!m->night)
        return 1.0f;
    sight = NIGHT_SIGHT * (p->flashlight ? FL_SIGHT_MULT : 1.0f);
    b = 1.0f - dist / sight;
    if (b < NIGHT_DARK_FLOOR)
        b = NIGHT_DARK_FLOOR;
    if (b > 1.0f)
        b = 1.0f;
    return b;
}

sfColor shade_color(sfColor c, float b)
{
    c.r = (sfUint8)(c.r * b);
    c.g = (sfUint8)(c.g * b);
    c.b = (sfUint8)(c.b * b);
    return c;
}
