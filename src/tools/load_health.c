/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** load_health.c
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

static int init_hp_text(player_t *p)
{
    sfVector2f pos = {p->ww - 95.0f, 14.0f};

    p->hp_txt = sfText_create();
    if (!p->hp_txt)
        return EXIT_FAIL;
    sfText_setFont(p->hp_txt, p->hud_font);
    sfText_setCharacterSize(p->hp_txt, HP_FONT_SZ);
    sfText_setFillColor(p->hp_txt, COL_TITLE);
    sfText_setPosition(p->hp_txt, pos);
    return EXIT_SUCCESS;
}

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
    if (init_hp_text(p) == EXIT_FAIL)
        return EXIT_FAIL;
    set_health_frame(p);
    return EXIT_SUCCESS;
}

void set_health_frame(player_t *p)
{
    sfIntRect rect = {0, 0, HEALTH_FRAME_W, HEALTH_FRAME_H};
    int hp = p->hp < 0 ? 0 : p->hp;
    int frame = ((PLAYER_HP - hp) * (HEALTH_FRAMES - 1)
        + PLAYER_HP / 2) / PLAYER_HP;
    char buf[16];

    if (frame > HEALTH_FRAMES - 1)
        frame = HEALTH_FRAMES - 1;
    rect.left = (frame % HEALTH_COLS) * HEALTH_FRAME_W;
    rect.top = (frame / HEALTH_COLS) * HEALTH_FRAME_H;
    if (p->health_spr)
        sfSprite_setTextureRect(p->health_spr, rect);
    if (p->hp_txt) {
        snprintf(buf, sizeof(buf), "%d", hp);
        sfText_setString(p->hp_txt, buf);
    }
}

void destroy_health_bar(player_t *p)
{
    if (p->hp_txt)
        sfText_destroy(p->hp_txt);
    if (p->health_spr)
        sfSprite_destroy(p->health_spr);
    if (p->health_tex)
        sfTexture_destroy(p->health_tex);
    p->hp_txt = NULL;
}
