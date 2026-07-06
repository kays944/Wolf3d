/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** popup.c -- floating "+N" pickup feedback
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

void spawn_popup(player_t *p, float x, float y, popup_t data)
{
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (p->popups[i].active)
            continue;
        p->popups[i].x = x;
        p->popups[i].y = y;
        p->popups[i].t = 0;
        p->popups[i].amount = data.amount;
        p->popups[i].kind = data.kind;
        p->popups[i].active = sfTrue;
        return;
    }
    p->popups[0] = data;
    p->popups[0].x = x;
    p->popups[0].y = y;
    p->popups[0].t = 0;
    p->popups[0].active = sfTrue;
}

void update_popups(player_t *p)
{
    for (int i = 0; i < MAX_POPUPS; i++) {
        if (!p->popups[i].active)
            continue;
        p->popups[i].t += p->dt;
        if (p->popups[i].t >= POPUP_LIFE)
            p->popups[i].active = sfFalse;
    }
}

static void draw_one_popup(sfRenderWindow *win, player_t *p, popup_t *pu)
{
    sfText *t = sfText_create();
    char buf[24] = {0};
    float k = pu->t / POPUP_LIFE;
    sfColor col = pu->kind == PACK_MEDKIT ? COL_POP_HP : COL_POP_AMMO;
    sfFloatRect b = {0};

    if (!t)
        return;
    col.a = (sfUint8)(255 * (1.0f - k));
    snprintf(buf, sizeof(buf), pu->kind == PACK_MEDKIT ? "+%d PV" : "+%d",
        pu->amount);
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, buf);
    sfText_setCharacterSize(t, POPUP_FONT);
    sfText_setFillColor(t, col);
    b = sfText_getLocalBounds(t);
    sfText_setPosition(t, (sfVector2f){pu->x - b.width / 2.0f,
            pu->y - POPUP_RISE * k});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}

void draw_popups(sfRenderWindow *win, player_t *p)
{
    for (int i = 0; i < MAX_POPUPS; i++)
        if (p->popups[i].active)
            draw_one_popup(win, p, &p->popups[i]);
}
