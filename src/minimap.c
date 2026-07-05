/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** minimap.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

typedef struct mini_s {
    float ox;
    float oy;
    float cell;
    float k;
} mini_t;

static void mini_geo(player_t *p, map_t *m, mini_t *g)
{
    g->cell = MINI_CELL;
    if (m->size_x * g->cell > p->ww * MINI_MAX_W)
        g->cell = p->ww * MINI_MAX_W / m->size_x;
    g->k = g->cell / MINI_CELL;
    g->ox = MINI_MARGIN;
    g->oy = MINI_TOP_OFFSET;
}

static void mini_dot(sfRenderWindow *win, sfVector2f c, float r, sfColor col)
{
    sfCircleShape *dot = sfCircleShape_create();

    if (!dot)
        return;
    sfCircleShape_setRadius(dot, r);
    sfCircleShape_setOrigin(dot, (sfVector2f){r, r});
    sfCircleShape_setPosition(dot, c);
    sfCircleShape_setFillColor(dot, col);
    sfRenderWindow_drawCircleShape(win, dot, NULL);
    sfCircleShape_destroy(dot);
}

static void draw_mini_bg(sfRenderWindow *win, map_t *m, mini_t *g)
{
    sfRectangleShape *bg = sfRectangleShape_create();

    if (!bg)
        return;
    sfRectangleShape_setPosition(bg,
        (sfVector2f){g->ox - 3.0f, g->oy - 3.0f});
    sfRectangleShape_setSize(bg, (sfVector2f){m->size_x * g->cell + 6.0f,
            m->size_y * g->cell + 6.0f});
    sfRectangleShape_setFillColor(bg, COL_MINI_BG);
    sfRenderWindow_drawRectangleShape(win, bg, NULL);
    sfRectangleShape_destroy(bg);
}

static void draw_mini_walls(sfRenderWindow *win, map_t *m, mini_t *g)
{
    sfRectangleShape *cell = sfRectangleShape_create();

    if (!cell)
        return;
    sfRectangleShape_setSize(cell, (sfVector2f){g->cell, g->cell});
    sfRectangleShape_setFillColor(cell, COL_MINI_WALL);
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++)
            if (m->map[y][x] == 'x' || m->map[y][x] == 'm'
                || m->map[y][x] == 'n') {
                sfRectangleShape_setPosition(cell, (sfVector2f){
                        g->ox + x * g->cell, g->oy + y * g->cell});
                sfRenderWindow_drawRectangleShape(win, cell, NULL);
            }
    sfRectangleShape_destroy(cell);
}

static sfColor enemy_mini_col(enemy_t *e)
{
    if (e->boss)
        return COL_MINI_BOSS;
    if (e->type == ENEMY_TYPE_BRUTE)
        return COL_MINI_BRUTE;
    if (e->type == ENEMY_TYPE_RUNNER)
        return COL_MINI_RUNNER;
    return COL_MINI_GRUNT;
}

static void draw_mini_enemies(sfRenderWindow *win, map_t *m, mini_t *g)
{
    enemy_t *e = NULL;
    sfVector2f c = {0};

    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive || e->dying)
            continue;
        c.x = g->ox + (e->x / TILE_SIZE) * g->cell;
        c.y = g->oy + (e->y / TILE_SIZE) * g->cell;
        mini_dot(win, c, (e->boss ? MINI_BOSS_DOT : MINI_DOT) * g->k,
            enemy_mini_col(e));
    }
}

static void draw_mini_markers(sfRenderWindow *win, map_t *m, mini_t *g)
{
    sfVector2f c = {0};

    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            c.x = g->ox + (x + 0.5f) * g->cell;
            c.y = g->oy + (y + 0.5f) * g->cell;
            if (m->map[y][x] == 'K')
                mini_dot(win, c, MINI_PLAYER_DOT * g->k, COL_KEY);
            if (m->map[y][x] == 'E')
                mini_dot(win, c, MINI_PLAYER_DOT * g->k, COL_EXIT);
            if (m->map[y][x] == 'D')
                mini_dot(win, c, MINI_DOT * g->k, COL_KEY);
            if (m->map[y][x] == 'd')
                mini_dot(win, c, MINI_DOT * g->k,
                    sfColor_fromRGB(170, 170, 180));
        }
}

static void draw_mini_player(sfRenderWindow *win, player_t *p, mini_t *g)
{
    sfVector2f c = {g->ox + (p->x / TILE_SIZE) * g->cell,
        g->oy + (p->y / TILE_SIZE) * g->cell};
    sfVector2f nose = {c.x + cosf(p->angle) * MINI_DIR_LEN * g->k,
        c.y + sinf(p->angle) * MINI_DIR_LEN * g->k};

    mini_dot(win, nose, MINI_DOT * g->k, COL_MINI_PLAYER);
    mini_dot(win, c, MINI_PLAYER_DOT * g->k, COL_MINI_PLAYER);
}

void draw_minimap(sfRenderWindow *win, player_t *p, map_t *m)
{
    mini_t g = {0};

    if (!m->map)
        return;
    mini_geo(p, m, &g);
    draw_mini_bg(win, m, &g);
    draw_mini_walls(win, m, &g);
    draw_mini_markers(win, m, &g);
    draw_mini_enemies(win, m, &g);
    draw_mini_player(win, p, &g);
}
