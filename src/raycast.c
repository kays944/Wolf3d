/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** raycast.c — DDA grid traversal, one step per tile crossed
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static int tile_kind(map_t *m, int mx, int my)
{
    if (mx < 0 || mx >= m->size_x || my < 0 || my >= m->size_y)
        return WALL_STONE;
    return wall_kind(m->map[my][mx]);
}

static void dda_init(dda_t *d, player_t *p, float angle)
{
    d->px = p->x / TILE_SIZE;
    d->py = p->y / TILE_SIZE;
    d->rdx = cosf(angle);
    d->rdy = sinf(angle);
    d->mx = (int)d->px;
    d->my = (int)d->py;
    d->ddx = d->rdx == 0 ? 1e30f : fabsf(1.0f / d->rdx);
    d->ddy = d->rdy == 0 ? 1e30f : fabsf(1.0f / d->rdy);
    d->stx = d->rdx < 0 ? -1 : 1;
    d->sty = d->rdy < 0 ? -1 : 1;
    d->sdx = (d->rdx < 0 ? d->px - d->mx : d->mx + 1.0f - d->px) * d->ddx;
    d->sdy = (d->rdy < 0 ? d->py - d->my : d->my + 1.0f - d->py) * d->ddy;
    d->side = 0;
}

static int dda_step(dda_t *d, map_t *m)
{
    if (d->sdx < d->sdy) {
        d->sdx += d->ddx;
        d->mx += d->stx;
        d->side = 0;
    } else {
        d->sdy += d->ddy;
        d->my += d->sty;
        d->side = 1;
    }
    return tile_kind(m, d->mx, d->my);
}

static float wall_frac(dda_t *d, float t)
{
    float wx = 0;

    if (d->side == 0)
        wx = d->py + t * d->rdy;
    else
        wx = d->px + t * d->rdx;
    return wx - floorf(wx);
}

float cast_ray_raw(player_t *p, float angle, map_t *m, wall_hit_t *hit)
{
    dda_t d;
    int kind = 0;
    float t = 0;

    dda_init(&d, p, angle);
    kind = tile_kind(m, d.mx, d.my);
    while (kind < 0) {
        kind = dda_step(&d, m);
        t = d.side == 0 ? d.sdx - d.ddx : d.sdy - d.ddy;
    }
    hit->kind = kind;
    hit->tex_x = wall_frac(&d, t);
    return t * TILE_SIZE;
}

float cast_wall_ray(player_t *p, float angle, map_t *m, wall_hit_t *hit)
{
    return cast_ray_raw(p, angle, m, hit) * cosf(p->angle - angle);
}
