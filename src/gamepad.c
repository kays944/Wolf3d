/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** gamepad.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static float pad_axis(sfJoystickAxis a)
{
    float v = sfJoystick_getAxisPosition(PAD_ID, a);

    if (v > -PAD_DEADZONE && v < PAD_DEADZONE)
        return 0;
    return v / 100.0f;
}

static void pad_move(player_t *p, map_t *m)
{
    float lx = pad_axis(sfJoystickX);
    float ly = pad_axis(sfJoystickY);
    float mag = sqrtf(lx * lx + ly * ly);

    if (mag == 0)
        return;
    if (mag > 1.0f)
        mag = 1.0f;
    player_step(p, m, p->angle + atan2f(lx, -ly), mag);
}

static void pad_look(player_t *p)
{
    float rx = pad_axis(sfJoystickU);
    float ry = pad_axis(sfJoystickV);

    p->angle += rx * ROTATION_SPEED * p->dt;
    p->pitch -= ry * PITCH_SPEED * p->dt;
}

void update_gamepad(player_t *p, map_t *m)
{
    if (!sfJoystick_isConnected(PAD_ID))
        return;
    pad_move(p, m);
    pad_look(p);
}
