/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** hud_fx.c
*/

#include <unistd.h>
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
    float len = p->aiming ? CROSS_SIZE_AIM : CROSS_SIZE;
    sfColor col = p->aiming ? sfColor_fromRGBA(90, 255, 120, 230)
        : sfColor_fromRGBA(255, 255, 255, 200);

    if (!r)
        return;
    sfRectangleShape_setFillColor(r, col);
    sfRectangleShape_setSize(r, (sfVector2f){len, CROSS_THICK});
    sfRectangleShape_setPosition(r, (sfVector2f){mid.x - len / 2,
            mid.y - CROSS_THICK / 2});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_setSize(r, (sfVector2f){CROSS_THICK, len});
    sfRectangleShape_setPosition(r, (sfVector2f){mid.x - CROSS_THICK / 2,
            mid.y - len / 2});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

static int level_won(player_t *p, map_t *m)
{
    if (m->has_exit)
        return p->reached_exit;
    return m->enemy_count > 0 && enemies_alive(m) == 0;
}

int check_game_end(sfRenderWindow *win, player_t *p, map_t *m)
{
    char path[MAP_NAME_LEN + 32] = {0};
    int mode = END_MODE_WIN_LAST;

    if (p->hp <= 0)
        return run_end_menu(win, p, END_MSG_LOSE, END_MODE_LOSE);
    if (level_won(p, m)) {
        if (next_level_path(m, path, sizeof(path)) == EXIT_SUCCESS
            && access(path, F_OK) == 0)
            mode = END_MODE_WIN_NEXT;
        return run_end_menu(win, p, END_MSG_WIN, mode);
    }
    return 0;
}
