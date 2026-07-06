/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** tex_floor_ceil.c — sky as one GPU quad, floor in fixed-point
*/

#include "macros.h"
#include "proto.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

static void init_ceil_ctx(ceil_ctx_t *c, player_t *p, sfImage *img)
{
    sfVector2u tsz = sfImage_getSize(img);

    c->fpx = (const sfUint32 *)sfImage_getPixelsPtr(img);
    c->cpx = p->ceil_pixels;
    c->tw = (int)tsz.x;
    c->th = (int)tsz.y;
    c->ww = p->ww;
    c->posZ = p->wh * (0.5f + p->z / TILE_SIZE);
    c->ldx = cosf(p->angle - FOV / 2.0f);
    c->ldy = sinf(p->angle - FOV / 2.0f);
    c->rdx = cosf(p->angle + FOV / 2.0f);
    c->rdy = sinf(p->angle + FOV / 2.0f);
    c->px = p->x / TILE_SIZE;
    c->py = p->y / TILE_SIZE;
}

static void fill_floor_row(ceil_ctx_t *c, int y, int by)
{
    double rd = c->posZ / (double)by;
    int64_t fx = (int64_t)((c->px + rd * c->ldx) * c->tw * 65536.0);
    int64_t fy = (int64_t)((c->py + rd * c->ldy) * c->th * 65536.0);
    int64_t sx = (int64_t)(rd * (c->rdx - c->ldx) * c->tw * 65536.0 / c->ww);
    int64_t sy = (int64_t)(rd * (c->rdy - c->ldy) * c->th * 65536.0 / c->ww);
    sfUint32 *dst = (sfUint32 *)(c->cpx + (size_t)y * c->ww * 4);

    for (int x = 0; x < c->ww; x++) {
        dst[x] = c->fpx[((fy >> 16) & (c->th - 1)) * c->tw
            + ((fx >> 16) & (c->tw - 1))];
        fx += sx;
        fy += sy;
    }
}

static void append_vert(sfVertexArray *va, float x, float y,
    const sfVector2f *tex)
{
    sfVertex v = {0};

    v.position = (sfVector2f){x, y};
    v.texCoords = *tex;
    v.color = sfWhite;
    sfVertexArray_append(va, v);
}

static void draw_sky_quad(sfRenderWindow *win, player_t *p, int horizon)
{
    sfVector2u tsz = sfTexture_getSize(p->sky_tex);
    float span = p->wh / 2.0f + p->wh / (float)PITCH_MAX_DIV;
    float u0 = (p->angle / (2.0f * M_PI) - FOV / (4.0f * M_PI)) * tsz.x;
    float u1 = u0 + FOV / (2.0f * M_PI) * tsz.x;
    float v0 = (span - horizon) / span * (tsz.y - 1);
    sfVertexArray *va = sfVertexArray_create();
    sfRenderStates rs = sfRenderStates_default();

    if (!va)
        return;
    sfVertexArray_setPrimitiveType(va, sfQuads);
    append_vert(va, 0, 0, &(sfVector2f){u0, v0});
    append_vert(va, p->ww, 0, &(sfVector2f){u1, v0});
    append_vert(va, p->ww, horizon, &(sfVector2f){u1, (float)tsz.y - 1});
    append_vert(va, 0, horizon, &(sfVector2f){u0, (float)tsz.y - 1});
    rs.texture = p->sky_tex;
    sfRenderWindow_drawVertexArray(win, va, &rs);
    sfVertexArray_destroy(va);
}

static void draw_floor_part(sfRenderWindow *win, player_t *p, int horizon)
{
    ceil_ctx_t c = {0};

    init_ceil_ctx(&c, p, p->floor_img);
    memset(&p->ceil_pixels[(size_t)horizon * p->ww * 4], 20, p->ww * 4);
    for (int y = horizon + 1; y < p->wh; y++)
        fill_floor_row(&c, y, y - horizon);
    sfTexture_updateFromPixels(p->ceil_tex,
        &p->ceil_pixels[(size_t)horizon * p->ww * 4],
        p->ww, p->wh - horizon, 0, horizon);
    sfSprite_setTextureRect(p->ceil_spr,
        (sfIntRect){0, horizon, p->ww, p->wh - horizon});
    sfSprite_setPosition(p->ceil_spr, (sfVector2f){0, (float)horizon});
    sfRenderWindow_drawSprite(win, p->ceil_spr, NULL);
}

void draw_background(sfRenderWindow *win, player_t *p)
{
    int horizon = p->wh / 2 + (int)p->pitch;

    if (horizon < 1)
        horizon = 1;
    if (horizon > p->wh - 1)
        horizon = p->wh - 1;
    draw_sky_quad(win, p, horizon);
    draw_floor_part(win, p, horizon);
}
