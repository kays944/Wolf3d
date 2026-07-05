/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** marker.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void draw_marker(sfRenderWindow *win, player_t *p, sfVector2f c,
    sfColor col)
{
    float dx = c.x - p->x;
    float dy = c.y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);
    float base = 0;
    float size = 0;
    float ybot = 0;
    sfRectangleShape *r = NULL;

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    base = (TILE_SIZE * p->wh) / dist;
    size = (MARK_WORLD_SIZE * p->wh) / dist;
    ybot = p->wh / 2.0f + p->pitch + base / 2.0f + base * (p->z / TILE_SIZE);
    r = sfRectangleShape_create();
    if (!r)
        return;
    sfRectangleShape_setSize(r, (sfVector2f){size, size * 1.7f});
    sfRectangleShape_setFillColor(r, col);
    sfRectangleShape_setOutlineThickness(r, size * 0.08f);
    sfRectangleShape_setOutlineColor(r, sfColor_fromRGB(15, 15, 15));
    sfRectangleShape_setPosition(r, (sfVector2f){(rel / FOV + 0.5f) * p->ww
            - size / 2.0f, ybot - size * 1.7f});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

void draw_markers(sfRenderWindow *win, player_t *p, map_t *m)
{
    sfVector2f c = {0};

    if (!p->zbuf)
        return;
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            c.x = x * TILE_SIZE + TILE_SIZE / 2.0f;
            c.y = y * TILE_SIZE + TILE_SIZE / 2.0f;
            if (m->map[y][x] == 'K')
                draw_marker(win, p, c, COL_KEY);
            if (m->map[y][x] == 'E')
                draw_marker(win, p, c, COL_EXIT);
        }
}
