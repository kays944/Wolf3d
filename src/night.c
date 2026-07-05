/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** night.c
*/

#include <math.h>
#include <stdio.h>
#include "macros.h"
#include "proto.h"

void init_night(map_t *m)
{
    m->night = sfFalse;
    m->night_cd = NIGHT_DELAY;
}

static void find_free_spot(map_t *m, float *x, float *y)
{
    static const int off[5][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {0, 0}};
    float nx = 0;
    float ny = 0;

    for (int i = 0; i < 5; i++) {
        nx = *x + off[i][0] * TILE_SIZE;
        ny = *y + off[i][1] * TILE_SIZE;
        if (is_blocked(nx, ny, m) != IS_WALL) {
            *x = nx;
            *y = ny;
            return;
        }
    }
}

static void spawn_night_wave(map_t *m)
{
    int base = m->enemy_count;
    enemy_t *e = NULL;
    float x = 0;
    float y = 0;

    for (int i = 0; i < base; i++) {
        e = &m->enemies[i];
        if (!e->alive || e->dying || e->boss)
            continue;
        x = e->x;
        y = e->y;
        find_free_spot(m, &x, &y);
        spawn_enemy(m, x, y, e->type);
    }
}

void update_night(player_t *p, map_t *m)
{
    if (m->night) {
        if (m->night_cd > -NIGHT_MSG_TIME)
            m->night_cd -= p->dt;
        return;
    }
    m->night_cd -= p->dt;
    if (m->night_cd <= 0) {
        m->night = sfTrue;
        spawn_night_wave(m);
    }
}

static void draw_center_text(sfRenderWindow *win, player_t *p,
    const char *msg, sfColor col)
{
    sfText *t = sfText_create();
    sfFloatRect lb = {0};

    if (!t)
        return;
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, msg);
    sfText_setCharacterSize(t, NIGHT_FONT_SZ);
    sfText_setFillColor(t, col);
    lb = sfText_getLocalBounds(t);
    sfText_setPosition(t, (sfVector2f){
        (p->ww - lb.width) / 2.0f - lb.left, p->wh * 0.16f});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}

void draw_night_hud(sfRenderWindow *win, player_t *p, map_t *m)
{
    char buf[48] = {0};

    if (!m->night) {
        snprintf(buf, sizeof(buf), "LA NUIT TOMBE DANS %d",
            (int)ceilf(m->night_cd));
        draw_center_text(win, p, buf, COL_NIGHT);
        return;
    }
    if (m->night_cd > -NIGHT_MSG_TIME)
        draw_center_text(win, p, "LA NUIT EST TOMBEE !",
            sfColor_fromRGB(235, 70, 45));
}
