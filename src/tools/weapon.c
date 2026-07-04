/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon.c
*/

#include "macros.h"
#include "proto.h"

void place_weapon_sprite(player_t *p)
{
    sfVector2u sz = sfTexture_getSize(p->weapon_idle);
    sfVector2f sc = {0};
    sfVector2f pos = {0};

    sc.x = (float)p->ww / 2.0f / sz.x;
    sc.y = sc.x;
    sfSprite_setScale(p->weapon_spr, sc);
    pos.x = (p->ww - sz.x * sc.x) / 2.0f + p->ww * WEAPON_X_RATIO;
    pos.y = p->wh - sz.y * sc.y;
    sfSprite_setPosition(p->weapon_spr, pos);
}

int init_weapon(player_t *p)
{
    p->weapon_idle = sfTexture_createFromFile("./assets/weapon_idle.png", NULL);
    p->weapon_fire = sfTexture_createFromFile("./assets/weapon_fire.png", NULL);
    if (!p->weapon_idle || !p->weapon_fire)
        return EXIT_FAIL;
    p->weapon_spr = sfSprite_create();
    p->weapon_clock = sfClock_create();
    if (!p->weapon_spr || !p->weapon_clock)
        return EXIT_FAIL;
    sfSprite_setTexture(p->weapon_spr, p->weapon_idle, sfTrue);
    place_weapon_sprite(p);
    p->firing = sfFalse;
    return EXIT_SUCCESS;
}

void destroy_weapon(player_t *p)
{
    if (p->weapon_spr)
        sfSprite_destroy(p->weapon_spr);
    if (p->weapon_clock)
        sfClock_destroy(p->weapon_clock);
    if (p->weapon_idle)
        sfTexture_destroy(p->weapon_idle);
    if (p->weapon_fire)
        sfTexture_destroy(p->weapon_fire);
}
