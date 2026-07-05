/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_draw.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static sfColor enemy_tint(enemy_t *e)
{
    if (e->boss)
        return sfWhite;
    if (e->type == ENEMY_TYPE_BRUTE)
        return sfColor_fromRGB(255, 140, 140);
    if (e->type == ENEMY_TYPE_RUNNER)
        return sfColor_fromRGB(150, 230, 255);
    return sfWhite;
}

static void append_vert(sfVertexArray *va, const sfVector2f *pos,
    const sfVector2f *tex, sfColor col)
{
    sfVertex v = {0};

    v.position = *pos;
    v.texCoords = *tex;
    v.color = col;
    sfVertexArray_append(va, v);
}

static void set_frame(spr_ctx_t *c, enemy_t *e)
{
    int col = 0;
    int row = 0;

    if (e->dying) {
        row = DEATH_ROW;
        col = (int)(e->death_t / DEATH_FRAME_LEN);
        col = col >= DEATH_FRAMES ? DEATH_FRAMES - 1 : col;
        c->cw = c->tsz.x / (float)ANIM_COLS;
        c->ch = c->tsz.y / (float)ANIM_ROWS;
        c->u0 = col * c->cw;
        c->v0 = row * c->ch;
        return;
    }
    if (e->atk_anim > 0) {
        row = 1;
        col = (int)((ATK_ANIM_LEN - e->atk_anim) / ATK_ANIM_LEN
            * ATK_FRAMES);
        col = col >= ATK_FRAMES ? ATK_FRAMES - 1 : col;
    } else if (e->moving)
        col = (int)(e->anim_t * WALK_FPS) % WALK_FRAMES;
    c->cw = c->tsz.x / (float)ANIM_COLS;
    c->ch = c->tsz.y / (float)ANIM_ROWS;
    c->u0 = col * c->cw;
    c->v0 = row * c->ch;
}

static void append_quad(spr_ctx_t *c, int i)
{
    float span = (float)(c->i1 - c->i0 + 1);
    float u = c->u0 + (i - c->i0) / span * c->cw;
    float du = c->cw / span;
    float x = i * c->col_w;
    float yt = c->ybot - c->size;
    float yb = c->ybot;

    append_vert(c->va, &(sfVector2f){x, yt}, &(sfVector2f){u, c->v0},
        c->tint);
    append_vert(c->va, &(sfVector2f){x + c->col_w, yt},
        &(sfVector2f){u + du, c->v0}, c->tint);
    append_vert(c->va, &(sfVector2f){x + c->col_w, yb},
        &(sfVector2f){u + du, c->v0 + c->ch}, c->tint);
    append_vert(c->va, &(sfVector2f){x, yb},
        &(sfVector2f){u, c->v0 + c->ch}, c->tint);
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
    set_frame(c, e);
    for (int i = c->i0; i <= c->i1; i++) {
        if (i < 0 || i >= NUM_RAYS || c->dist >= p->zbuf[i])
            continue;
        append_quad(c, i);
    }
    sfRenderWindow_drawVertexArray(win, c->va, &rs);
    sfVertexArray_destroy(c->va);
}

static void fill_bar(sfRenderWindow *win, sfRectangleShape *r,
    const sfFloatRect *box, sfColor col)
{
    sfRectangleShape_setSize(r, (sfVector2f){box->width, box->height});
    sfRectangleShape_setPosition(r, (sfVector2f){box->left, box->top});
    sfRectangleShape_setFillColor(r, col);
    sfRenderWindow_drawRectangleShape(win, r, NULL);
}

static void draw_hp_bar(sfRenderWindow *win, spr_ctx_t *c,
    enemy_t *e, player_t *p)
{
    float ratio = e->max_hp > 0 ? (float)e->hp / e->max_hp : 0.0f;
    float bh = c->size * HPBAR_H_RATIO;
    float bw = c->width * HPBAR_W_RATIO;
    sfFloatRect box = {0};
    sfRectangleShape *r = NULL;
    int mid = (c->i0 + c->i1) / 2;

    if (e->dying)
        return;
    if (mid < 0 || mid >= NUM_RAYS || c->dist >= p->zbuf[mid])
        return;
    r = sfRectangleShape_create();
    if (!r)
        return;
    bh = bh < HPBAR_MIN_H ? HPBAR_MIN_H : bh;
    box = (sfFloatRect){c->x0 + (c->width - bw) / 2.0f,
        c->ybot - c->size - bh - HPBAR_GAP, bw, bh};
    fill_bar(win, r, &box, sfColor_fromRGBA(25, 25, 25, 200));
    box.width = bw * ratio;
    fill_bar(win, r, &box, sfColor_fromRGB(255 * (1.0f - ratio),
        210 * ratio + 30, 25));
    sfRectangleShape_destroy(r);
}

static void draw_one(sfRenderWindow *win, enemy_t *e, player_t *p)
{
    spr_ctx_t c = {0};
    float dx = e->x - p->x;
    float dy = e->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float base = 0;
    float width = 0;

    if (fabsf(rel) > FOV / 2 + 0.5f)
        return;
    c.tint = enemy_tint(e);
    c.dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    if (c.dist < DISTANCE_LIMIT * 8)
        return;
    base = (TILE_SIZE * p->wh) / c.dist;
    c.size = base * (e->boss ? BOSS_SCALE : 1.0f);
    c.ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    c.col_w = p->ww / (float)NUM_RAYS;
    c.tsz = sfTexture_getSize(e->boss ? p->boss_tex : p->enemy_tex);
    width = c.size * ((c.tsz.x / (float)ANIM_COLS)
        / (c.tsz.y / (float)ANIM_ROWS));
    c.width = width;
    c.x0 = (rel / FOV + 0.5f) * p->ww - width / 2.0f;
    c.i0 = (int)(c.x0 / c.col_w);
    c.i1 = (int)((c.x0 + width) / c.col_w);
    render_strips(win, &c, e, p);
    draw_hp_bar(win, &c, e, p);
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
