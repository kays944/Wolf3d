/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** hud_fx.c
*/

#include "macros.h"
#include "proto.h"

void draw_hurt_flash(sfRenderWindow *win, player_t *p)
{
    sfRectangleShape *r = NULL;

    if (p->hurt_flash <= 0)
        return;
    p->hurt_flash--;
    r = sfRectangleShape_create();
    if (!r)
        return;
    sfRectangleShape_setSize(r, (sfVector2f){p->ww, p->wh});
    sfRectangleShape_setFillColor(r, sfColor_fromRGBA(190, 10, 10, 95));
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

void draw_crosshair(sfRenderWindow *win, player_t *p)
{
    sfRectangleShape *r = sfRectangleShape_create();
    sfVector2f mid = {p->ww / 2.0f, p->wh / 2.0f};

    if (!r)
        return;
    sfRectangleShape_setFillColor(r, sfColor_fromRGBA(255, 255, 255, 200));
    sfRectangleShape_setSize(r, (sfVector2f){CROSS_SIZE, CROSS_THICK});
    sfRectangleShape_setPosition(r, (sfVector2f){mid.x - CROSS_SIZE / 2,
            mid.y - CROSS_THICK / 2});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_setSize(r, (sfVector2f){CROSS_THICK, CROSS_SIZE});
    sfRectangleShape_setPosition(r, (sfVector2f){mid.x - CROSS_THICK / 2,
            mid.y - CROSS_SIZE / 2});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

static sfText *make_end_text(player_t *p, const char *msg)
{
    sfText *t = sfText_create();
    sfFloatRect b = {0};

    if (!t)
        return NULL;
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, msg);
    sfText_setCharacterSize(t, END_FONT_SZ);
    sfText_setFillColor(t, COL_TITLE);
    b = sfText_getGlobalBounds(t);
    sfText_setPosition(t, (sfVector2f){(p->ww - b.width) / 2.0f,
            (p->wh - b.height) / 2.0f - b.top});
    return t;
}

static void end_screen_frame(sfRenderWindow *win, sfText *t)
{
    sfEvent ev = {0};

    while (sfRenderWindow_pollEvent(win, &ev))
        if (ev.type == sfEvtClosed)
            sfRenderWindow_close(win);
    sfRenderWindow_clear(win, sfBlack);
    sfRenderWindow_drawText(win, t, NULL);
    sfRenderWindow_display(win);
}

void show_end_screen(sfRenderWindow *win, player_t *p, const char *msg)
{
    sfText *t = make_end_text(p, msg);
    sfClock *ck = sfClock_create();

    if (!t || !ck) {
        if (t)
            sfText_destroy(t);
        if (ck)
            sfClock_destroy(ck);
        return;
    }
    while (sfRenderWindow_isOpen(win) && sfTime_asMilliseconds(
            sfClock_getElapsedTime(ck)) < END_SCREEN_MS)
        end_screen_frame(win, t);
    sfText_destroy(t);
    sfClock_destroy(ck);
}

int check_game_end(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (p->hp <= 0) {
        show_end_screen(win, p, END_MSG_LOSE);
        return 1;
    }
    if (m->enemy_count > 0 && enemies_alive(m) == 0) {
        show_end_screen(win, p, END_MSG_WIN);
        return 1;
    }
    return 0;
}
