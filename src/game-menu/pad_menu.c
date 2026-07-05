/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pad_menu.c
*/

#include "macros.h"
#include "proto.h"

static int axis_group(sfJoystickAxis a, int *vert)
{
    if (a == sfJoystickY || a == sfJoystickPovY) {
        *vert = 1;
        return 1;
    }
    if (a == sfJoystickX || a == sfJoystickPovX) {
        *vert = 0;
        return 1;
    }
    return 0;
}

static int latch_dir(float v, int *latch, int neg, int pos)
{
    if (v > -PAD_MENU_LO && v < PAD_MENU_LO) {
        *latch = 0;
        return PM_NONE;
    }
    if (*latch)
        return PM_NONE;
    if (v <= -PAD_MENU_HI) {
        *latch = 1;
        return neg;
    }
    if (v >= PAD_MENU_HI) {
        *latch = 1;
        return pos;
    }
    return PM_NONE;
}

int pad_menu_action(sfEvent *e)
{
    static int latch_v = 0;
    static int latch_h = 0;
    int vert = 0;

    if (e->type == sfEvtJoystickButtonPressed) {
        if (e->joystickButton.button == PAD_BTN_OK)
            return PM_OK;
        if (e->joystickButton.button == PAD_BTN_MENU_BACK)
            return PM_BACK;
        return PM_NONE;
    }
    if (e->type != sfEvtJoystickMoved
        || !axis_group(e->joystickMove.axis, &vert))
        return PM_NONE;
    if (vert)
        return latch_dir(e->joystickMove.position, &latch_v,
            PM_UP, PM_DOWN);
    return latch_dir(e->joystickMove.position, &latch_h,
        PM_LEFT, PM_RIGHT);
}
