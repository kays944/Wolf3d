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
        copy_texel(c, (ty * c->tw + tx) * 4, (y * c->ww + x) * 4);
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

static void draw_sky_part(player_t *p, int horizon)
{
    float span = p->wh / 2.0f + p->wh / (float)PITCH_MAX_DIV;
    sfVector2u tsz = sfImage_getSize(p->sky_img);
    float fov_ratio = FOV / (2.0f * M_PI);
    sky_row_t sr = {sfImage_getPixelsPtr(p->sky_img), (int)tsz.x,
        p->angle / (2.0f * M_PI) - fov_ratio * 0.5f,
        fov_ratio / p->ww, p->ww};
    float tyf = 0;
    int ty = 0;

    for (int y = 0; y < horizon && y < p->wh; y++) {
        tyf = (y - horizon + span) / span;
        if (tyf < 0)
            tyf = 0;
        ty = (int)(tyf * (tsz.y - 1)) % tsz.y;
        fill_sky_row(&p->ceil_pixels[y * p->ww * 4], &sr, ty);
    }
}

static void draw_floor_part(player_t *p, int horizon)
{
    ceil_ctx_t c = {0};

    init_ceil_ctx(&c, p, p->wall_img);
    for (int y = horizon + 1; y < p->wh; y++)
        fill_floor_row(&c, y, y - horizon);
    if (horizon >= 0 && horizon < p->wh)
        memset(&p->ceil_pixels[horizon * p->ww * 4], 20, p->ww * 4);
}

void draw_background(sfRenderWindow *win, player_t *p)
{
    int horizon = p->wh / 2 + (int)p->pitch;

    if (horizon < 1)
        horizon = 1;
    if (horizon > p->wh - 1)
        horizon = p->wh - 1;
    draw_sky_part(p, horizon);
    draw_floor_part(p, horizon);
    sfTexture_updateFromPixels(p->ceil_tex, p->ceil_pixels, p->ww,
        p->wh, 0, 0);
    sfRenderWindow_drawSprite(win, p->ceil_spr, NULL);
}
