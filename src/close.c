/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** close.c
*/

#include "wolf.h"

void close_all(sfRenderWindow *window)
{
    sfRenderWindow_destroy(window);
}
