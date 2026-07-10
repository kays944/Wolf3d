/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** reload.c — procedural reload animation (weapon dips while mag swaps)
*/

#include "macros.h"
#include "proto.h"

int init_reload(player_t *p)
{
    p->reload_clock = sfClock_create();
    if (!p->reload_clock)
        return EXIT_FAIL;
    p->reloading = sfFalse;
    return EXIT_SUCCESS;
}

void destroy_reload(player_t *p)
{
    if (p->reload_clock)
        sfClock_destroy(p->reload_clock);
    p->reload_clock = NULL;
}

void start_reload(player_t *p, sound_t *s)
{
    if (p->reloading || p->firing)
        return;
    if (p->ammo >= AMMO_DEFAULT || p->reserve <= 0)
        return;
    play_reload(s);
    p->reloading = sfTrue;
    sfClock_restart(p->reload_clock);
}

static float dip_amount(float t)
{
    float d = 1.0f;

    if (t < RELOAD_DIP_T)
        d = t / RELOAD_DIP_T;
    if (t > RELOAD_TIME - RELOAD_DIP_T)
        d = (RELOAD_TIME - t) / RELOAD_DIP_T;
    return d * d * (3.0f - 2.0f * d);
}

static void place_reload_sprite(player_t *p, float t)
{
    float d = dip_amount(t);
    sfVector2f pos = {0};

    place_weapon_sprite(p);
    pos = sfSprite_getPosition(p->weapon_spr);
    pos.y += d * ((float)p->wh - pos.y) * RELOAD_DIP_FRAC;
    sfSprite_setPosition(p->weapon_spr, pos);
    sfSprite_setRotation(p->weapon_spr, d * RELOAD_TILT);
}

static void finish_reload(player_t *p)
{
    p->reloading = sfFalse;
    sfSprite_setRotation(p->weapon_spr, 0);
    place_weapon_sprite(p);
    reload_ammo(p);
}

void update_reload(player_t *p)
{
    float t = 0;

    if (!p->reloading)
        return;
    t = sfTime_asSeconds(sfClock_getElapsedTime(p->reload_clock));
    if (t >= RELOAD_TIME) {
        finish_reload(p);
        return;
    }
    place_reload_sprite(p, t);
}
