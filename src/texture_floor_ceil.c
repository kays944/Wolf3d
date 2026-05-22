/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** tex_floor_ceil.c
*/

#include "macros.h"
#include "proto.h"
#include <math.h>

static void copy_texel(ceil_ctx_t *c, int wi, int pi)
{
    c->cpx[pi] = c->wpx[wi];
    c->cpx[pi + 1] = c->wpx[wi + 1];
    c->cpx[pi + 2] = c->wpx[wi + 2];
    c->cpx[pi + 3] = c->wpx[wi + 3];
}

static void init_ceil_ctx(ceil_ctx_t *c, player_t *p)
{
    sfVector2u tsz = sfImage_getSize(p->wall_img);

    c->wpx = sfImage_getPixelsPtr(p->wall_img);
    c->cpx = p->ceil_pixels;
    c->tw = (int)tsz.x;
    c->th = (int)tsz.y;
    c->ww = p->ww;
    c->posZ = p->wh / 2.0f;
    c->ldx = cosf(p->angle - FOV / 2.0f);
    c->ldy = sinf(p->angle - FOV / 2.0f);
    c->rdx = cosf(p->angle + FOV / 2.0f);
    c->rdy = sinf(p->angle + FOV / 2.0f);
    c->px = p->x / TILE_SIZE;
    c->py = p->y / TILE_SIZE;
}

static void fill_ceil_row(ceil_ctx_t *c, int y)
{
    float rd = c->posZ / (c->posZ - (float)y);
    float fx = c->px + rd * c->ldx;
    float fy = c->py + rd * c->ldy;
    float sx = rd * (c->rdx - c->ldx) / c->ww;
    float sy = rd * (c->rdy - c->ldy) / c->ww;
    int tx;
    int ty;

    for (int x = 0; x < c->ww; x++) {
        tx = ((int)(fx * c->tw) % c->tw + c->tw) % c->tw;
        ty = ((int)(fy * c->th) % c->th + c->th) % c->th;
        copy_texel(c, (ty * c->tw + tx) * 4, (y * c->ww + x) * 4);
        fx += sx;
        fy += sy;
    }
}

static void fill_floor_row(ceil_ctx_t *c, int by)
{
    float rd = c->posZ / (float)by;
    float fx = c->px + rd * c->ldx;
    float fy = c->py + rd * c->ldy;
    float sx = rd * (c->rdx - c->ldx) / c->ww;
    float sy = rd * (c->rdy - c->ldy) / c->ww;
    int tx;
    int ty;

    for (int x = 0; x < c->ww; x++) {
        tx = ((int)(fx * c->tw) % c->tw + c->tw) % c->tw;
        ty = ((int)(fy * c->th) % c->th + c->th) % c->th;
        copy_texel(c, (ty * c->tw + tx) * 4, (by * c->ww + x) * 4);
        fx += sx;
        fy += sy;
    }
}

void draw_ceil_tex(sfRenderWindow *win, player_t *p)
{
    ceil_ctx_t c = {0};
    int half_h = p->wh / 2;

    init_ceil_ctx(&c, p);
    for (int y = 0; y < half_h; y++)
        fill_ceil_row(&c, y);
    sfTexture_updateFromPixels(p->ceil_tex, p->ceil_pixels,
        p->ww, half_h, 0, 0);
    sfRenderWindow_drawSprite(win, p->ceil_spr, NULL);
}

void draw_floor_tex(sfRenderWindow *win, player_t *p)
{
    ceil_ctx_t c = {0};
    int half_h = p->wh / 2;

    init_ceil_ctx(&c, p);
    for (int by = 1; by < half_h; by++)
        fill_floor_row(&c, by);
    sfTexture_updateFromPixels(p->floor_tex, p->ceil_pixels,
        p->ww, half_h, 0, 0);
    sfRenderWindow_drawSprite(win, p->floor_spr, NULL);
}
