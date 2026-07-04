/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pickup.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void scan_pack_row(map_t *m, int row)
{
    for (int j = 0; m->map[row][j]; j++) {
        if (m->map[row][j] != 'h' || m->pack_count >= MAX_PACKS)
            continue;
        m->packs[m->pack_count].x = j * TILE_SIZE + TILE_SIZE / 2;
        m->packs[m->pack_count].y = row * TILE_SIZE + TILE_SIZE / 2;
        m->packs[m->pack_count].active = sfTrue;
        m->pack_count++;
    }
}

int init_pickups(player_t *p, map_t *m)
{
    m->pack_count = 0;
    for (int i = 0; i < m->size_y; i++)
        scan_pack_row(m, i);
    p->pack_tex = sfTexture_createFromFile(PACK_TEX_PATH, NULL);
    p->pack_spr = sfSprite_create();
    if (!p->pack_tex || !p->pack_spr) {
        destroy_pickups(p);
        return EXIT_FAIL;
    }
    sfSprite_setTexture(p->pack_spr, p->pack_tex, sfTrue);
    return EXIT_SUCCESS;
}

void destroy_pickups(player_t *p)
{
    if (p->pack_spr)
        sfSprite_destroy(p->pack_spr);
    if (p->pack_tex)
        sfTexture_destroy(p->pack_tex);
    p->pack_spr = NULL;
    p->pack_tex = NULL;
}

void update_pickups(player_t *p, map_t *m)
{
    pickup_t *pk = NULL;
    float dx = 0;
    float dy = 0;

    if (p->hp >= PLAYER_HP)
        return;
    for (int i = 0; i < m->pack_count; i++) {
        pk = &m->packs[i];
        if (!pk->active)
            continue;
        dx = pk->x - p->x;
        dy = pk->y - p->y;
        if (dx * dx + dy * dy > PACK_RADIUS * PACK_RADIUS)
            continue;
        pk->active = sfFalse;
        p->hp = p->hp + PACK_HP > PLAYER_HP ? PLAYER_HP : p->hp + PACK_HP;
        set_health_frame(p);
    }
}

static void draw_one_pack(sfRenderWindow *win, pickup_t *pk, player_t *p)
{
    float dx = pk->x - p->x;
    float dy = pk->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    float size = 0;
    float ybot = 0;
    float base = 0;
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);
    sfVector2u tsz = sfTexture_getSize(p->pack_tex);

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    base = (TILE_SIZE * p->wh) / dist;
    size = (PACK_WORLD_SIZE * p->wh) / dist;
    ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    sfSprite_setScale(p->pack_spr, (sfVector2f){size / tsz.x,
            size / tsz.y});
    sfSprite_setPosition(p->pack_spr, (sfVector2f){(rel / FOV + 0.5f)
            * p->ww - size / 2.0f, ybot - size});
    sfRenderWindow_drawSprite(win, p->pack_spr, NULL);
}

void draw_pickups(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (!p->pack_tex || !p->pack_spr || !p->zbuf)
        return;
    for (int i = 0; i < m->pack_count; i++)
        if (m->packs[i].active)
            draw_one_pack(win, &m->packs[i], p);
}
