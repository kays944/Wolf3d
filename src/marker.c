/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** marker.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

typedef struct mark_geo_s {
    float rel;
    float dist;
    float size;
    float ybot;
} mark_geo_t;

static int marker_geo(player_t *p, sfVector2f c, mark_geo_t *g)
{
    float dx = c.x - p->x;
    float dy = c.y - p->y;
    float base = 0;
    int ray = 0;

    g->rel = norm_angle(atan2f(dy, dx) - p->angle);
    g->dist = sqrtf(dx * dx + dy * dy) * cosf(g->rel);
    ray = (int)((g->rel / FOV + 0.5f) * NUM_RAYS);
    if (fabsf(g->rel) > FOV / 2 + 0.3f || g->dist < 8.0f)
        return 0;
    if (ray < 0 || ray >= NUM_RAYS || g->dist >= p->zbuf[ray])
        return 0;
    base = (TILE_SIZE * p->wh) / g->dist;
    g->size = (MARK_WORLD_SIZE * p->wh) / g->dist;
    g->ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    return 1;
}

static void draw_marker(sfRenderWindow *win, player_t *p, sfVector2f c,
    sfColor col)
{
    mark_geo_t g = {0};
    sfRectangleShape *r = NULL;

    if (!marker_geo(p, c, &g))
        return;
    r = sfRectangleShape_create();
    if (!r)
        return;
    sfRectangleShape_setSize(r, (sfVector2f){g.size, g.size * 1.7f});
    sfRectangleShape_setFillColor(r, col);
    sfRectangleShape_setOutlineThickness(r, g.size * 0.08f);
    sfRectangleShape_setOutlineColor(r, sfColor_fromRGB(15, 15, 15));
    sfRectangleShape_setPosition(r, (sfVector2f){(g.rel / FOV + 0.5f)
            * p->ww - g.size / 2.0f, g.ybot - g.size * 1.7f});
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

static void draw_key_marker(sfRenderWindow *win, player_t *p, sfVector2f c)
{
    mark_geo_t g = {0};
    sfVector2u tsz;
    float scale = 0;

    if (!marker_geo(p, c, &g))
        return;
    sfSprite_setTexture(p->key_spr, p->key_tex, sfTrue);
    tsz = sfTexture_getSize(p->key_tex);
    scale = g.size / (float)tsz.x;
    sfSprite_setScale(p->key_spr, (sfVector2f){scale, scale});
    sfSprite_setPosition(p->key_spr, (sfVector2f){(g.rel / FOV + 0.5f)
            * p->ww - g.size / 2.0f, g.ybot - tsz.y * scale});
    sfRenderWindow_drawSprite(win, p->key_spr, NULL);
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
            if (m->map[y][x] == 'K' && p->key_spr && p->key_tex)
                draw_key_marker(win, p, c);
            if (m->map[y][x] == 'E')
                draw_marker(win, p, c, COL_EXIT);
        }
}
