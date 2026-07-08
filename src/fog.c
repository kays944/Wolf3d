/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** fog.c — minimap fog of war (seen-tiles grid revealed around player)
*/

#include <stdlib.h>
#include <math.h>
#include "macros.h"
#include "proto.h"

int init_fog(map_t *m)
{
    if (m->seen)
        free(m->seen);
    m->seen = calloc((size_t)(m->size_x * m->size_y), 1);
    if (!m->seen)
        return EXIT_FAIL;
    return EXIT_SUCCESS;
}

int fog_seen(map_t *m, int x, int y)
{
    if (!m->seen)
        return 1;
    if (x < 0 || x >= m->size_x || y < 0 || y >= m->size_y)
        return 0;
    return m->seen[y * m->size_x + x];
}

static void reveal_row(map_t *m, int px, int y, int dy)
{
    int span = 0;

    if (y < 0 || y >= m->size_y)
        return;
    span = (int)(sqrtf((float)(FOG_RADIUS * FOG_RADIUS - dy * dy)) + 0.5f);
    for (int x = px - span; x <= px + span; x++)
        if (x >= 0 && x < m->size_x)
            m->seen[y * m->size_x + x] = 1;
}

void update_fog(player_t *p, map_t *m)
{
    int px = (int)(p->x / TILE_SIZE);
    int py = (int)(p->y / TILE_SIZE);

    if (!m->seen)
        return;
    for (int dy = -FOG_RADIUS; dy <= FOG_RADIUS; dy++)
        reveal_row(m, px, py + dy, dy);
}
