/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wall_tex.c
*/

#include <stdlib.h>
#include "macros.h"
#include "proto.h"

static int init_ceil_spr(player_t *p)
{
    p->ceil_tex = sfTexture_create(p->ww, p->wh);
    if (!p->ceil_tex)
        return EXIT_FAIL;
    p->ceil_spr = sfSprite_create();
    if (!p->ceil_spr) {
        sfTexture_destroy(p->ceil_tex);
        return EXIT_FAIL;
    }
    sfSprite_setTexture(p->ceil_spr, p->ceil_tex, sfTrue);
    sfSprite_setPosition(p->ceil_spr, (sfVector2f){0, 0});
    return EXIT_SUCCESS;
}

static int init_ceil_resources(player_t *p)
{
    p->ceil_pixels = malloc(p->ww * p->wh * 4);
    if (!p->ceil_pixels)
        return EXIT_FAIL;
    if (init_ceil_spr(p) == EXIT_FAIL) {
        free(p->ceil_pixels);
        p->ceil_pixels = NULL;
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static int init_wall_images(player_t *p)
{
    p->wall_img = sfImage_createFromFile(WALL_TEX_PATH);
    if (!p->wall_img) {
        sfTexture_destroy(p->wall_tex);
        return EXIT_FAIL;
    }
    p->sky_img = sfImage_createFromFile(SKY_TEX_PATH);
    if (!p->sky_img) {
        sfImage_destroy(p->wall_img);
        sfTexture_destroy(p->wall_tex);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

int init_wall_tex(player_t *p)
{
    p->wall_tex = sfTexture_createFromFile(WALL_TEX_PATH, NULL);
    if (!p->wall_tex)
        return EXIT_FAIL;
    sfTexture_setRepeated(p->wall_tex, sfTrue);
    if (init_wall_images(p) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_ceil_resources(p) == EXIT_FAIL) {
        sfImage_destroy(p->sky_img);
        sfImage_destroy(p->wall_img);
        sfTexture_destroy(p->wall_tex);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static void destroy_floor_ceil(player_t *p)
{
    if (p->floor_spr)
        sfSprite_destroy(p->floor_spr);
    p->floor_spr = NULL;
    if (p->floor_tex)
        sfTexture_destroy(p->floor_tex);
    p->floor_tex = NULL;
    if (p->ceil_spr)
        sfSprite_destroy(p->ceil_spr);
    p->ceil_spr = NULL;
    if (p->ceil_tex)
        sfTexture_destroy(p->ceil_tex);
    p->ceil_tex = NULL;
    if (p->ceil_pixels)
        free(p->ceil_pixels);
    p->ceil_pixels = NULL;
}

void destroy_wall_tex(player_t *p)
{
    destroy_floor_ceil(p);
    if (p->sky_img)
        sfImage_destroy(p->sky_img);
    p->sky_img = NULL;
    if (p->wall_img)
        sfImage_destroy(p->wall_img);
    p->wall_img = NULL;
    if (p->wall_tex)
        sfTexture_destroy(p->wall_tex);
    p->wall_tex = NULL;
}
