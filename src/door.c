/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** door.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void add_door(map_t *m, int tx, int ty, sfBool locked)
{
    if (m->door_count >= MAX_DOORS)
        return;
    m->doors[m->door_count].tx = tx;
    m->doors[m->door_count].ty = ty;
    m->doors[m->door_count].locked = locked;
    m->doors[m->door_count].open = sfFalse;
    m->door_count++;
}

void init_doors(map_t *m)
{
    m->door_count = 0;
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            if (m->map[y][x] == 'd')
                add_door(m, x, y, sfFalse);
            if (m->map[y][x] == 'D')
                add_door(m, x, y, sfTrue);
        }
}

static float door_dist(door_t *d, player_t *p)
{
    float cx = d->tx * TILE_SIZE + TILE_SIZE / 2.0f;
    float cy = d->ty * TILE_SIZE + TILE_SIZE / 2.0f;

    return sqrtf((cx - p->x) * (cx - p->x) + (cy - p->y) * (cy - p->y));
}

static void set_open(map_t *m, door_t *d, sfBool open)
{
    d->open = open;
    if (open)
        m->map[d->ty][d->tx] = ' ';
    else
        m->map[d->ty][d->tx] = d->locked ? 'D' : 'd';
}

static void update_one_door(map_t *m, door_t *d, player_t *p)
{
    float dist = door_dist(d, p);

    if (d->locked) {
        if (!d->open && dist < DOOR_OPEN_DIST && p->has_key)
            set_open(m, d, sfTrue);
        return;
    }
    if (dist < DOOR_OPEN_DIST)
        set_open(m, d, sfTrue);
    else if (d->open)
        set_open(m, d, sfFalse);
}

void update_doors(player_t *p, map_t *m)
{
    for (int i = 0; i < m->door_count; i++)
        update_one_door(m, &m->doors[i], p);
}
