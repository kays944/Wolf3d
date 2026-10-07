/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** keyexit.c
*/

#include "macros.h"
#include "proto.h"

void init_keyexit(map_t *m)
{
    m->has_exit = sfFalse;
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++)
            if (m->map[y][x] == 'E')
                m->has_exit = sfTrue;
}

static float tile_dist2(player_t *p, int x, int y)
{
    float cx = x * TILE_SIZE + TILE_SIZE / 2.0f;
    float cy = y * TILE_SIZE + TILE_SIZE / 2.0f;

    return (cx - p->x) * (cx - p->x) + (cy - p->y) * (cy - p->y);
}

void update_keyexit(player_t *p, map_t *m)
{
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            if (m->map[y][x] == 'K'
                && tile_dist2(p, x, y) < KEY_PICK_DIST * KEY_PICK_DIST) {
                p->has_key = sfTrue;
                m->map[y][x] = ' ';
            }
            if (m->map[y][x] == 'E'
                && tile_dist2(p, x, y) < EXIT_DIST * EXIT_DIST)
                p->reached_exit = sfTrue;
        }
}

void draw_key_hint(sfRenderWindow *win, player_t *p)
{
    sfText *t = NULL;

    if (!p->has_key)
        return;
    t = sfText_create();
    if (!t)
        return;
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, "KEY");
    sfText_setCharacterSize(t, 26);
    sfText_setFillColor(t, COL_KEY);
    sfText_setPosition(t, (sfVector2f){20.0f, 48.0f});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}
