/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wall_tex.c
*/

#include "macros.h"
#include "proto.h"

int init_wall_tex(player_t *p)
{
    p->wall_tex = sfTexture_createFromFile(WALL_TEX_PATH, NULL);
    if (!p->wall_tex)
        return EXIT_FAIL;
    sfTexture_setRepeated(p->wall_tex, sfTrue);
    return EXIT_SUCCESS;
}

void destroy_wall_tex(player_t *p)
{
    if (p->wall_tex)
        sfTexture_destroy(p->wall_tex);
    p->wall_tex = NULL;
}
