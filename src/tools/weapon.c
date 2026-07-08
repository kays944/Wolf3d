/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** weapon.c
*/

#include "macros.h"
#include "proto.h"

sfTexture *weapon_idle_tex(player_t *p)
{
    return p->weapon == WEAPON_PISTOL ? p->pistol_idle : p->weapon_idle;
}

sfTexture *weapon_fire_tex(player_t *p)
{
    return p->weapon == WEAPON_PISTOL ? p->pistol_fire : p->weapon_fire;
}

void switch_weapon(player_t *p)
{
    if (p->firing || p->reloading)
        return;
    p->weapon = (p->weapon == WEAPON_PISTOL) ? WEAPON_RIFLE : WEAPON_PISTOL;
    sfSprite_setTexture(p->weapon_spr, weapon_idle_tex(p), sfTrue);
    place_weapon_sprite(p);
    refresh_ammo_text(p);
}

void place_weapon_sprite(player_t *p)
{
    sfVector2u sz = sfTexture_getSize(weapon_idle_tex(p));
    sfVector2f sc = {0};
    sfVector2f pos = {0};

    sc.x = (float)p->ww / 2.0f / sz.x * (p->aiming ? AIM_ZOOM : 1.0f);
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
    p->pistol_idle = sfTexture_createFromFile(PISTOL_IDLE_PATH, NULL);
    p->pistol_fire = sfTexture_createFromFile(PISTOL_FIRE_PATH, NULL);
    if (!p->weapon_idle || !p->weapon_fire
        || !p->pistol_idle || !p->pistol_fire)
        return EXIT_FAIL;
    p->weapon = WEAPON_RIFLE;
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
    if (p->pistol_idle)
        sfTexture_destroy(p->pistol_idle);
    if (p->pistol_fire)
        sfTexture_destroy(p->pistol_fire);
}
