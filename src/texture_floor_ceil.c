/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** tex_floor_ceil.c
*/

#include "macros.h"
#include "proto.h"
#include <math.h>
#include <string.h>

static void copy_texel(ceil_ctx_t *c, int wi, int pi)
{
    c->cpx[pi] = c->wpx[wi];
    c->cpx[pi + 1] = c->wpx[wi + 1];
    c->cpx[pi + 2] = c->wpx[wi + 2];
    c->cpx[pi + 3] = c->wpx[wi + 3];
}

static void init_ceil_ctx(ceil_ctx_t *c, player_t *p, sfImage *img)
{
    sfVector2u tsz = sfImage_getSize(img);

    c->wpx = sfImage_getPixelsPtr(img);
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

static void fill_floor_row(ceil_ctx_t *c, int by)
{
    float rd = c->posZ / (float)by;
    float fx = c->px + rd * c->ldx;
    float fy = c->py + rd * c->ldy;
    float sx = rd * (c->rdx - c->ldx) / c->ww;
    float sy = rd * (c->rdy - c->ldy) / c->ww;
    int tx = 0;
    int ty = 0;

    for (int x = 0; x < c->ww; x++) {
        tx = ((int)(fx * c->tw) % c->tw + c->tw) % c->tw;
        ty = ((int)(fy * c->th) % c->th + c->th) % c->th;
        copy_texel(c, (ty * c->tw + tx) * 4, (by * c->ww + x) * 4);
        fx += sx;
        fy += sy;
    }
}

static void fill_sky_row(sfUint8 *dst, const sky_row_t *sr, int ty)
{
    int row_base = ty * sr->tw;
    float u = 0;
    int tx = 0;

    for (int x = 0; x < sr->ww; x++) {
        u = fmodf(sr->u_base + x * sr->u_step, 1.0f);
        if (u < 0)
            u += 1.0f;
        tx = (int)(u * sr->tw) % sr->tw;
        memcpy(&dst[x * 4], &sr->spx[(row_base + tx) * 4], 4);
    }
}

void draw_ceil_tex(sfRenderWindow *win, player_t *p)
{
    int half_h = p->wh / 2;
    sfVector2u tsz = sfImage_getSize(p->sky_img);
    int tw = (int)tsz.x;
    int th = (int)tsz.y;
    float fov_ratio = FOV / (2.0f * M_PI);
    sky_row_t sr = {sfImage_getPixelsPtr(p->sky_img), tw,
        p->angle / (2.0f * M_PI) - fov_ratio * 0.5f,
        fov_ratio / p->ww, p->ww};
    int ty;

    for (int y = 0; y < half_h; y++) {
        ty = (int)((float)y / half_h * th) % th;
        fill_sky_row(&p->ceil_pixels[y * p->ww * 4], &sr, ty);
    }
    sfTexture_updateFromPixels(p->ceil_tex, p->ceil_pixels, p->ww,
        half_h, 0, 0);
    sfRenderWindow_drawSprite(win, p->ceil_spr, NULL);
}

void draw_floor_tex(sfRenderWindow *win, player_t *p)
{
    ceil_ctx_t c = {0};
    int half_h = p->wh / 2;

    init_ceil_ctx(&c, p, p->wall_img);
    for (int by = 1; by < half_h; by++)
        fill_floor_row(&c, by);
    sfTexture_updateFromPixels(p->floor_tex, p->ceil_pixels,
        p->ww, half_h, 0, 0);
    sfRenderWindow_drawSprite(win, p->floor_spr, NULL);
}
