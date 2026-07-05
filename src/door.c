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

static door_t *door_near(player_t *p, map_t *m)
{
    door_t *best = NULL;
    float best_d = DOOR_OPEN_DIST;
    float dist = 0;

    for (int i = 0; i < m->door_count; i++) {
        if (m->doors[i].open)
            continue;
        dist = door_dist(&m->doors[i], p);
        if (dist < best_d) {
            best_d = dist;
            best = &m->doors[i];
        }
    }
    return best;
}

int try_open_door(player_t *p, map_t *m)
{
    door_t *d = door_near(p, m);

    if (!d)
        return 0;
    if (d->locked && !p->has_key)
        return 1;
    d->open = sfTrue;
    m->map[d->ty][d->tx] = ' ';
    return 1;
}

static void center_hint(player_t *p, sfText *t)
{
    sfFloatRect lb = sfText_getLocalBounds(t);

    sfText_setPosition(t, (sfVector2f){
        (p->ww - lb.width) / 2.0f - lb.left,
        p->wh * DOOR_HINT_Y});
}

void draw_door_hint(sfRenderWindow *win, player_t *p, map_t *m)
{
    door_t *d = door_near(p, m);
    sfText *t = NULL;
    sfBool no_key = sfFalse;

    if (!d)
        return;
    no_key = d->locked && !p->has_key;
    t = sfText_create();
    if (!t)
        return;
    sfText_setFont(t, p->hud_font);
    sfText_setCharacterSize(t, DOOR_HINT_SZ);
    if (no_key)
        sfText_setString(t, "IL FAUT UNE CLE");
    else
        sfText_setString(t, p->use_pad ? "CARRE : OUVRIR" : "E : OUVRIR");
    sfText_setFillColor(t, no_key ? sfColor_fromRGB(235, 70, 45) : COL_KEY);
    center_hint(p, t);
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}
