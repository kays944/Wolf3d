/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_ai.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static float dist_to_player(enemy_t *e, player_t *p)
{
    float dx = p->x - e->x;
    float dy = p->y - e->y;

    return sqrtf(dx * dx + dy * dy);
}

static void chase(enemy_t *e, player_t *p, map_t *m)
{
    float dist = dist_to_player(e, p);
    float step = ENEMY_SPEED * p->dt;
    float nx = 0;
    float ny = 0;

    if (dist <= ENEMY_STOP_DIST || dist > ENEMY_SIGHT)
        return;
    nx = e->x + (p->x - e->x) / dist * step;
    ny = e->y + (p->y - e->y) / dist * step;
    if (is_wall(nx, ny, m) != IS_WALL) {
        e->x = nx;
        e->y = ny;
        return;
    }
    if (is_wall(nx, e->y, m) != IS_WALL) {
        e->x = nx;
        return;
    }
    if (is_wall(e->x, ny, m) != IS_WALL)
        e->y = ny;
}

static void hurt_player(player_t *p, int dmg)
{
    p->hp -= dmg;
    p->hurt_flash = HURT_FLASH_FRAMES;
    for (int i = 0; i < dmg; i++)
        if (p->health_spr)
            decrement_health(p->health_spr);
}

static int hit_chance(float dist)
{
    int chance = HIT_BASE - (int)(dist * HIT_FALL);

    if (chance < HIT_MIN)
        return HIT_MIN;
    return chance;
}

static void try_shoot(enemy_t *e, player_t *p, sound_t *s)
{
    float dist = dist_to_player(e, p);

    if (dist > ENEMY_SHOOT_RANGE || e->cooldown > 0)
        return;
    e->cooldown = ENEMY_SHOOT_CD;
    play_enemy_shot(s);
    if ((rand() % 100) >= hit_chance(dist))
        return;
    hurt_player(p, e->boss ? BOSS_DMG : ENEMY_DMG);
}

void update_enemies(player_t *p, map_t *m, sound_t *s)
{
    enemy_t *e = NULL;

    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive)
            continue;
        if (e->cooldown > 0)
            e->cooldown -= p->dt;
        if (!has_los(e->x, e->y, p, m))
            continue;
        chase(e, p, m);
        try_shoot(e, p, s);
    }
}
