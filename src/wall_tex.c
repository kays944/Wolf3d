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

static const char *WALL_PATHS[WALL_KINDS] = {
    WALL_TEX_PATH,
    BRICK_TEX_PATH,
    COLD_TEX_PATH,
    DOOR_TEX_PATH,
    DOOR_LOCKED_TEX_PATH,
};

static void destroy_wall_texs(player_t *p)
{
    for (int k = 0; k < WALL_KINDS; k++) {
        if (p->wall_texs[k])
            sfTexture_destroy(p->wall_texs[k]);
        p->wall_texs[k] = NULL;
    }
}

static int init_wall_texs(player_t *p)
{
    for (int k = 0; k < WALL_KINDS; k++) {
        p->wall_texs[k] = sfTexture_createFromFile(WALL_PATHS[k], NULL);
        if (!p->wall_texs[k]) {
            destroy_wall_texs(p);
            return EXIT_FAIL;
        }
        sfTexture_setRepeated(p->wall_texs[k], sfTrue);
    }
    return EXIT_SUCCESS;
}

static int pow2_size(sfImage *img)
{
    sfVector2u sz = sfImage_getSize(img);

    return sz.x > 0 && sz.y > 0
        && (sz.x & (sz.x - 1)) == 0 && (sz.y & (sz.y - 1)) == 0;
}

static int load_skies(player_t *p)
{
    p->sky_tex = sfTexture_createFromFile(SKY_TEX_PATH, NULL);
    p->sky_night_tex = sfTexture_createFromFile(SKY_NIGHT_TEX_PATH, NULL);
    if (!p->sky_tex || !p->sky_night_tex) {
        if (p->sky_tex)
            sfTexture_destroy(p->sky_tex);
        p->sky_tex = NULL;
        return EXIT_FAIL;
    }
    sfTexture_setRepeated(p->sky_tex, sfTrue);
    sfTexture_setRepeated(p->sky_night_tex, sfTrue);
    return EXIT_SUCCESS;
}

static int init_wall_images(player_t *p)
{
    p->floor_img = sfImage_createFromFile(FLOOR_TEX_PATH);
    if (!p->floor_img || !pow2_size(p->floor_img)) {
        destroy_wall_texs(p);
        return EXIT_FAIL;
    }
    if (load_skies(p) == EXIT_FAIL) {
        sfImage_destroy(p->floor_img);
        destroy_wall_texs(p);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

int init_wall_tex(player_t *p)
{
    if (init_wall_texs(p) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_wall_images(p) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_ceil_resources(p) == EXIT_FAIL) {
        sfTexture_destroy(p->sky_tex);
        sfTexture_destroy(p->sky_night_tex);
        sfImage_destroy(p->floor_img);
        destroy_wall_texs(p);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static void destroy_floor_ceil(player_t *p)
{
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
    if (p->sky_tex)
        sfTexture_destroy(p->sky_tex);
    p->sky_tex = NULL;
    if (p->sky_night_tex)
        sfTexture_destroy(p->sky_night_tex);
    p->sky_night_tex = NULL;
    if (p->floor_img)
        sfImage_destroy(p->floor_img);
    p->floor_img = NULL;
    destroy_wall_texs(p);
}
