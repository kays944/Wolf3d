/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_draw.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void append_vert(sfVertexArray *va, const sfVector2f *pos,
    const sfVector2f *tex)
{
    sfVertex v = {0};

    v.position = *pos;
    v.texCoords = *tex;
    v.color = sfWhite;
    sfVertexArray_append(va, v);
}

static void append_quad(spr_ctx_t *c, int i)
{
    float span = (float)(c->i1 - c->i0 + 1);
    float u = (i - c->i0) / span * c->tsz.x;
    float du = c->tsz.x / span;
    float x = i * c->col_w;
    float yt = c->ybot - c->size;
    float yb = c->ybot;

    append_vert(c->va, &(sfVector2f){x, yt}, &(sfVector2f){u, 0});
    append_vert(c->va, &(sfVector2f){x + c->col_w, yt},
        &(sfVector2f){u + du, 0});
    append_vert(c->va, &(sfVector2f){x + c->col_w, yb},
        &(sfVector2f){u + du, c->tsz.y});
    append_vert(c->va, &(sfVector2f){x, yb}, &(sfVector2f){u, c->tsz.y});
}

static void render_strips(sfRenderWindow *win, spr_ctx_t *c,
    enemy_t *e, player_t *p)
{
    sfRenderStates rs = sfRenderStates_default();

    c->va = sfVertexArray_create();
    if (!c->va)
        return;
    sfVertexArray_setPrimitiveType(c->va, sfQuads);
    rs.texture = e->boss ? p->boss_tex : p->enemy_tex;
    c->tsz = sfTexture_getSize(rs.texture);
    for (int i = c->i0; i <= c->i1; i++) {
        if (i < 0 || i >= NUM_RAYS || c->dist >= p->zbuf[i])
            continue;
        append_quad(c, i);
    }
    sfRenderWindow_drawVertexArray(win, c->va, &rs);
    sfVertexArray_destroy(c->va);
}

static void draw_one(sfRenderWindow *win, enemy_t *e, player_t *p)
{
    spr_ctx_t c = {0};
    float dx = e->x - p->x;
    float dy = e->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float base = 0;

    if (fabsf(rel) > FOV / 2 + 0.5f)
        return;
    c.dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    if (c.dist < DISTANCE_LIMIT * 8)
        return;
    base = (TILE_SIZE * p->wh) / c.dist;
    c.size = base * (e->boss ? BOSS_SCALE : 1.0f);
    c.ybot = p->wh / 2.0f + base / 2.0f;
    c.col_w = p->ww / (float)NUM_RAYS;
    c.x0 = (rel / FOV + 0.5f) * p->ww - c.size / 2.0f;
    c.i0 = (int)(c.x0 / c.col_w);
    c.i1 = (int)((c.x0 + c.size) / c.col_w);
    render_strips(win, &c, e, p);
}

static int collect_alive(map_t *m, enemy_t **arr)
{
    int n = 0;

    for (int i = 0; i < m->enemy_count; i++) {
        if (!m->enemies[i].alive)
            continue;
        arr[n] = &m->enemies[i];
        n++;
    }
    return n;
}

void draw_enemies(sfRenderWindow *win, player_t *p, map_t *m)
{
    enemy_t *arr[MAX_ENEMIES] = {0};
    int n = collect_alive(m, arr);

    if (n == 0 || !p->enemy_tex || !p->boss_tex || !p->zbuf)
        return;
    sort_far(arr, n, p);
    for (int i = 0; i < n; i++)
        draw_one(win, arr[i], p);
}
