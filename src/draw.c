/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** creation.c
*/

#include "wolf.h"

void draw(sfRenderWindow *window)
{
    sfRenderWindow_clear(window, sfColor_fromRGB(45, 45, 55));
    sfRenderWindow_display(window);
}
