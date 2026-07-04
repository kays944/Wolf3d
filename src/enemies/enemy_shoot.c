/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_shoot.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static int aim_hit(enemy_t *e, player_t *p, float radius)
{
    float dx = e->x - p->x;
    float dy = e->y - p->y;
    float dist = sqrtf(dx * dx + dy * dy);
    float rel = norm_angle(atan2f(dy, dx) - p->angle);

    if (dist > SHOT_RANGE)
        return 0;
    return fabsf(rel) < atan2f(radius, dist);
}

static void damage_enemy(enemy_t *e, player_t *p)
{
    float scale = e->boss ? BOSS_SCALE : 1.0f;
    int dmg = SHOT_DMG;

    if (aim_hit(e, p, HEAD_RADIUS * scale))
        dmg = HEADSHOT_DMG;
    e->hp -= dmg;
    if (e->hp <= 0) {
        e->dying = sfTrue;
        e->death_t = 0;
    }
}

void shoot_enemies(player_t *p, map_t *m)
{
    enemy_t *e = NULL;
    float scale = 1.0f;

    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        scale = e->boss ? BOSS_SCALE : 1.0f;
        if (!e->alive || e->dying
            || !aim_hit(e, p, ENEMY_RADIUS * scale))
            continue;
        if (!has_los(e->x, e->y, p, m))
            continue;
        damage_enemy(e, p);
    }
}
