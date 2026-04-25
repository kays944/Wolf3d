/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_cleanup.c
*/

#include "game_proto.h"

void game_cleanup(game_t *g)
{
    if (g->font_big)
        sfFont_destroy(g->font_big);
    if (g->window)
        sfRenderWindow_destroy(g->window);
}
