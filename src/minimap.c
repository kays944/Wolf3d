/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** minimap.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void mini_origin(player_t *p, map_t *m, float *ox, float *oy)
{
    *ox = p->ww - MINI_MARGIN - m->size_x * MINI_CELL;
    *oy = MINI_MARGIN;
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

static void draw_mini_bg(sfRenderWindow *win, map_t *m, float ox, float oy)
{
    sfRectangleShape *bg = sfRectangleShape_create();

    if (!bg)
        return;
    sfRectangleShape_setPosition(bg, (sfVector2f){ox - 3.0f, oy - 3.0f});
    sfRectangleShape_setSize(bg, (sfVector2f){m->size_x * MINI_CELL + 6.0f,
            m->size_y * MINI_CELL + 6.0f});
    sfRectangleShape_setFillColor(bg, COL_MINI_BG);
    sfRenderWindow_drawRectangleShape(win, bg, NULL);
    sfRectangleShape_destroy(bg);
}

static void draw_mini_walls(sfRenderWindow *win, map_t *m, float ox, float oy)
{
    sfRectangleShape *cell = sfRectangleShape_create();

    if (!cell)
        return;
    sfRectangleShape_setSize(cell, (sfVector2f){MINI_CELL, MINI_CELL});
    sfRectangleShape_setFillColor(cell, COL_MINI_WALL);
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++)
            if (m->map[y][x] == 'x' || m->map[y][x] == 'm'
                || m->map[y][x] == 'n') {
                sfRectangleShape_setPosition(cell, (sfVector2f){
                        ox + x * MINI_CELL, oy + y * MINI_CELL});
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

static void draw_mini_enemies(sfRenderWindow *win, map_t *m, float ox, float oy)
{
    enemy_t *e = NULL;
    sfVector2f c = {0};

    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive || e->dying)
            continue;
        c.x = ox + (e->x / TILE_SIZE) * MINI_CELL;
        c.y = oy + (e->y / TILE_SIZE) * MINI_CELL;
        mini_dot(win, c, e->boss ? MINI_BOSS_DOT : MINI_DOT,
            enemy_mini_col(e));
    }
}

static void draw_mini_markers(sfRenderWindow *win, map_t *m, float ox,
    float oy)
{
    sfVector2f c = {0};

    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            c.x = ox + (x + 0.5f) * MINI_CELL;
            c.y = oy + (y + 0.5f) * MINI_CELL;
            if (m->map[y][x] == 'K')
                mini_dot(win, c, MINI_PLAYER_DOT, COL_KEY);
            if (m->map[y][x] == 'E')
                mini_dot(win, c, MINI_PLAYER_DOT, COL_EXIT);
            if (m->map[y][x] == 'D')
                mini_dot(win, c, MINI_DOT, COL_KEY);
        }
}

static void draw_mini_player(sfRenderWindow *win, player_t *p, float ox,
    float oy)
{
    sfVector2f c = {ox + (p->x / TILE_SIZE) * MINI_CELL,
        oy + (p->y / TILE_SIZE) * MINI_CELL};
    sfVector2f nose = {c.x + cosf(p->angle) * MINI_DIR_LEN,
        c.y + sinf(p->angle) * MINI_DIR_LEN};

    mini_dot(win, nose, MINI_DOT, COL_MINI_PLAYER);
    mini_dot(win, c, MINI_PLAYER_DOT, COL_MINI_PLAYER);
}

void draw_minimap(sfRenderWindow *win, player_t *p, map_t *m)
{
    float ox = 0;
    float oy = 0;

    if (!m->map)
        return;
    mini_origin(p, m, &ox, &oy);
    draw_mini_bg(win, m, ox, oy);
    draw_mini_walls(win, m, ox, oy);
    draw_mini_markers(win, m, ox, oy);
    draw_mini_enemies(win, m, ox, oy);
    draw_mini_player(win, p, ox, oy);
}
