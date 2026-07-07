/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** props.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static const char PROP_CHARS[PROP_TYPES] = {'p', 't', 'k', 'g'};

static const char *PROP_PATHS[PROP_TYPES] = {
    "./assets/props/barrel.png",
    "./assets/props/candelabra.png",
    "./assets/props/skeleton.png",
    "./assets/props/gore.png",
};

static const float PROP_SIZES[PROP_TYPES] = {44.0f, 58.0f, 62.0f, 48.0f};

void kill_prop(map_t *m, prop_t *pr)
{
    int tx = (int)pr->x / TILE_SIZE;
    int ty = (int)pr->y / TILE_SIZE;

    pr->dead = sfTrue;
    pr->fuse = 0;
    if (m->map[ty][tx] == 'p')
        m->map[ty][tx] = ' ';
}

int is_blocked(float x, float y, map_t *m)
{
    int tx = (int)x / TILE_SIZE;
    int ty = (int)y / TILE_SIZE;

    if (is_wall(x, y, m) == IS_WALL)
        return IS_WALL;
    if (m->map[ty][tx] == 'p')
        return IS_WALL;
    return EXIT_SUCCESS;
}

static void scan_prop_row(map_t *m, int row)
{
    for (int j = 0; m->map[row][j]; j++)
        for (int t = 0; t < PROP_TYPES; t++) {
            if (m->map[row][j] != PROP_CHARS[t]
                || m->prop_count >= MAX_PROPS)
                continue;
            m->props[m->prop_count].x = j * TILE_SIZE + TILE_SIZE / 2;
            m->props[m->prop_count].y = row * TILE_SIZE + TILE_SIZE / 2;
            m->props[m->prop_count].type = t;
            m->props[m->prop_count].dead = sfFalse;
            m->props[m->prop_count].fuse = 0;
            m->prop_count++;
        }
}

int init_props(player_t *p, map_t *m)
{
    m->prop_count = 0;
    for (int i = 0; i < m->size_y; i++)
        scan_prop_row(m, i);
    p->prop_spr = sfSprite_create();
    if (!p->prop_spr)
        return EXIT_FAIL;
    for (int t = 0; t < PROP_TYPES; t++) {
        p->prop_tex[t] = sfTexture_createFromFile(PROP_PATHS[t], NULL);
        if (!p->prop_tex[t]) {
            destroy_props(p);
            return EXIT_FAIL;
        }
    }
    return EXIT_SUCCESS;
}

void destroy_props(player_t *p)
{
    if (p->prop_spr)
        sfSprite_destroy(p->prop_spr);
    p->prop_spr = NULL;
    for (int t = 0; t < PROP_TYPES; t++) {
        if (p->prop_tex[t])
            sfTexture_destroy(p->prop_tex[t]);
        p->prop_tex[t] = NULL;
    }
}

static void draw_one_prop(sfRenderWindow *win, prop_t *pr, player_t *p,
    map_t *m)
{
    float dx = pr->x - p->x;
    float dy = pr->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    float size = 0;
    float ybot = 0;
    float base = 0;
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);
    sfVector2u tsz;

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    base = (TILE_SIZE * p->wh) / dist;
    size = (PROP_SIZES[pr->type] * p->wh) / dist;
    ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    sfSprite_setTexture(p->prop_spr, p->prop_tex[pr->type], sfTrue);
    sfSprite_setColor(p->prop_spr, shade_color(sfWhite,
            world_shade(dist, m, p)));
    tsz = sfTexture_getSize(p->prop_tex[pr->type]);
    sfSprite_setScale(p->prop_spr, (sfVector2f){size / tsz.x,
            size / tsz.y});
    sfSprite_setPosition(p->prop_spr, (sfVector2f){(rel / FOV + 0.5f)
            * p->ww - size / 2.0f, ybot - size});
    sfRenderWindow_drawSprite(win, p->prop_spr, NULL);
}

void draw_props(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (!p->prop_spr || !p->zbuf)
        return;
    for (int i = 0; i < m->prop_count; i++)
        if (!m->props[i].dead)
            draw_one_prop(win, &m->props[i], p, m);
}
