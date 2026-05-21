/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** ammo.c
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

static void set_ammo_pos(player_t *p)
{
    sfFloatRect lb = sfText_getLocalBounds(p->ammo_txt);
    sfVector2f pos = {0};

    pos.x = p->ww - lb.width - lb.left - 20.0f;
    pos.y = p->wh - lb.height - lb.top - 30.0f;
    sfText_setPosition(p->ammo_txt, pos);
}

int init_ammo(player_t *p)
{
    char buf[16] = {0};

    p->ammo = AMMO_DEFAULT;
    p->ammo_txt = sfText_create();
    if (!p->ammo_txt)
        return EXIT_FAIL;
    snprintf(buf, sizeof(buf), "MUN  %d", p->ammo);
    sfText_setFont(p->ammo_txt, p->hud_font);
    sfText_setString(p->ammo_txt, buf);
    sfText_setCharacterSize(p->ammo_txt, AMMO_FONT_SZ);
    sfText_setFillColor(p->ammo_txt, COL_TITLE);
    set_ammo_pos(p);
    return EXIT_SUCCESS;
}

void decrement_ammo(player_t *p)
{
    char buf[16] = {0};

    if (p->ammo <= 0)
        return;
    --p->ammo;
    snprintf(buf, sizeof(buf), "MUN  %d", p->ammo);
    sfText_setString(p->ammo_txt, buf);
    set_ammo_pos(p);
}

void destroy_ammo(player_t *p)
{
    if (p->ammo_txt)
        sfText_destroy(p->ammo_txt);
    p->ammo_txt = NULL;
}
