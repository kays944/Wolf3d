/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** projectile.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static proj_t *free_slot(map_t *m)
{
    for (int i = 0; i < MAX_PROJS; i++)
        if (!m->projs[i].active)
            return &m->projs[i];
    return NULL;
}

void spawn_proj(map_t *m, enemy_t *e, player_t *p)
{
    proj_t *pr = free_slot(m);
    float dx = p->x - e->x;
    float dy = p->y - e->y;
    float d = sqrtf(dx * dx + dy * dy);

    if (!pr || d < 1.0f)
        return;
    pr->x = e->x;
    pr->y = e->y;
    pr->dx = dx / d * PROJ_SPEED;
    pr->dy = dy / d * PROJ_SPEED;
    pr->boss = e->boss;
    pr->active = sfTrue;
}

static void move_proj(proj_t *pr, player_t *p, map_t *m)
{
    float ddx = 0;
    float ddy = 0;

    pr->x += pr->dx * p->dt;
    pr->y += pr->dy * p->dt;
    if (is_wall(pr->x, pr->y, m) == IS_WALL) {
        pr->active = sfFalse;
        return;
    }
    ddx = pr->x - p->x;
    ddy = pr->y - p->y;
    if (ddx * ddx + ddy * ddy > PROJ_HIT_RADIUS * PROJ_HIT_RADIUS)
        return;
    if (p->z < PROJ_DODGE_Z) {
        hurt_player(p, pr->boss ? BOSS_DMG : ENEMY_DMG);
        pr->active = sfFalse;
    }
}

void update_projs(player_t *p, map_t *m)
{
    for (int i = 0; i < MAX_PROJS; i++)
        if (m->projs[i].active)
            move_proj(&m->projs[i], p, m);
}

static void draw_one_proj(sfRenderWindow *win, proj_t *pr, player_t *p)
{
    float dx = pr->x - p->x;
    float dy = pr->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    float size = 0;
    float cy = 0;
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);
    sfVector2u tsz = sfTexture_getSize(p->proj_tex);

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    size = (PROJ_WORLD_SIZE * p->wh) / dist;
    cy = p->wh / 2.0f + p->pitch
        + (TILE_SIZE * p->wh / dist) * (p->z / TILE_SIZE);
    sfSprite_setScale(p->proj_spr, (sfVector2f){size / tsz.x,
            size / tsz.y});
    sfSprite_setPosition(p->proj_spr, (sfVector2f){(rel / FOV + 0.5f)
            * p->ww - size / 2.0f, cy - size / 2.0f});
    sfRenderWindow_drawSprite(win, p->proj_spr, NULL);
}

void draw_projs(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (!p->proj_tex || !p->proj_spr || !p->zbuf)
        return;
    for (int i = 0; i < MAX_PROJS; i++)
        if (m->projs[i].active)
            draw_one_proj(win, &m->projs[i], p);
}
