/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** load_health.c
*/

#include "macros.h"
#include "proto.h"

int init_health_bar(player_t *p)
{
    sfIntRect rect = {0, 0, HEALTH_FRAME_W, HEALTH_FRAME_H};
    sfVector2f scale = {0.5f, 0.5f};
    sfVector2f pos = {p->ww - HEALTH_FRAME_W * scale.x - 110.0f, 10.0f};

    p->health_tex = sfTexture_createFromFile(HEALTH_BAR_PATH, NULL);
    p->health_spr = sfSprite_create();
    if (!p->health_tex || !p->health_spr)
        return EXIT_FAIL;
    sfSprite_setTexture(p->health_spr, p->health_tex, sfTrue);
    sfSprite_setTextureRect(p->health_spr, rect);
    sfSprite_setPosition(p->health_spr, pos);
    sfSprite_setScale(p->health_spr, scale);
    return EXIT_SUCCESS;
}

void decrement_health(sfSprite *sprite)
{
    sfIntRect rect = sfSprite_getTextureRect(sprite);
    int col = rect.left / HEALTH_FRAME_W;
    int row = rect.top / HEALTH_FRAME_H;
    int frame = row * HEALTH_COLS + col;

    if (frame >= HEALTH_FRAMES - 1)
        return;
    frame++;
    rect.left = (frame % HEALTH_COLS) * HEALTH_FRAME_W;
    rect.top = (frame / HEALTH_COLS) * HEALTH_FRAME_H;
    sfSprite_setTextureRect(sprite, rect);
}

void destroy_health_bar(player_t *p)
{
    if (p->health_spr)
        sfSprite_destroy(p->health_spr);
    if (p->health_tex)
        sfTexture_destroy(p->health_tex);
}
