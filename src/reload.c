/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** reload.c
*/

#include "macros.h"
#include "proto.h"

static void set_reload_frame(player_t *p)
{
    sfIntRect rect = {0};
    sfVector2f sc = {0};
    sfVector2f pos = {0};
    int col = p->reload_frame % RELOAD_COLS;
    int row = p->reload_frame / RELOAD_COLS;

    rect.left = col * RELOAD_FRAME_W;
    rect.top = row * RELOAD_FRAME_H;
    rect.width = RELOAD_FRAME_W;
    rect.height = RELOAD_FRAME_H;
    sfSprite_setTextureRect(p->weapon_spr, rect);
    sc.x = (float)p->ww * 0.7f / (float)RELOAD_FRAME_W;
    sc.y = sc.x;
    sfSprite_setScale(p->weapon_spr, sc);
    pos.x = (p->ww - RELOAD_FRAME_W * sc.x) / 2.0f;
    pos.y = p->wh - RELOAD_FRAME_H * sc.y * 0.88f;
    sfSprite_setPosition(p->weapon_spr, pos);
}

int init_reload(player_t *p)
{
    p->reload_tex = sfTexture_createFromFile(RELOAD_TEX_PATH, NULL);
    if (!p->reload_tex)
        return EXIT_FAIL;
    p->reload_clock = sfClock_create();
    if (!p->reload_clock)
        return EXIT_FAIL;
    p->reloading = sfFalse;
    p->reload_frame = 0;
    return EXIT_SUCCESS;
}

void destroy_reload(player_t *p)
{
    if (p->reload_clock)
        sfClock_destroy(p->reload_clock);
    if (p->reload_tex)
        sfTexture_destroy(p->reload_tex);
    p->reload_clock = NULL;
    p->reload_tex = NULL;
}

void start_reload(player_t *p, sound_t *s)
{
    if (p->reloading || p->firing)
        return;
    if (p->ammo >= AMMO_DEFAULT || p->reserve <= 0)
        return;
    play_reload(s);
    p->reloading = sfTrue;
    p->reload_frame = 0;
    sfSprite_setTexture(p->weapon_spr, p->reload_tex, sfFalse);
    set_reload_frame(p);
    sfClock_restart(p->reload_clock);
}

void update_reload(player_t *p)
{
    sfTime t = {0};
    float ms = 0;

    if (!p->reloading)
        return;
    t = sfClock_getElapsedTime(p->reload_clock);
    ms = (float)sfTime_asMilliseconds(t);
    if (ms < RELOAD_FRAME_MS)
        return;
    sfClock_restart(p->reload_clock);
    p->reload_frame++;
    if (p->reload_frame >= RELOAD_FRAME_COUNT) {
        p->reloading = sfFalse;
        p->reload_frame = 0;
        sfSprite_setTexture(p->weapon_spr, p->weapon_idle, sfTrue);
        place_weapon_sprite(p);
        reload_ammo(p);
        return;
    }
    set_reload_frame(p);
}
