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
    e->moving = sfTrue;
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

void hurt_player(player_t *p, int dmg)
{
    if (p->hurt_cd > 0)
        return;
    p->hurt_cd = HURT_COOLDOWN;
    p->hp -= dmg;
    p->hurt_flash = HURT_FLASH_FRAMES;
    set_health_frame(p);
}

static void try_shoot(enemy_t *e, map_t *m, player_t *p, sound_t *s)
{
    float dist = dist_to_player(e, p);

    if (dist > ENEMY_SHOOT_RANGE || e->cooldown > 0)
        return;
    e->cooldown = ENEMY_SHOOT_CD;
    e->atk_anim = ATK_ANIM_LEN;
    play_enemy_shot(s);
    spawn_proj(m, e, p);
}

void update_enemies(player_t *p, map_t *m, sound_t *s)
{
    enemy_t *e = NULL;

    if (p->hurt_cd > 0)
        p->hurt_cd -= p->dt;
    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive)
            continue;
        if (e->dying) {
            e->death_t += p->dt;
            continue;
        }
        e->anim_t += p->dt;
        e->moving = sfFalse;
        if (e->cooldown > 0)
            e->cooldown -= p->dt;
        if (e->atk_anim > 0)
            e->atk_anim -= p->dt;
        if (!has_los(e->x, e->y, p, m))
            continue;
        chase(e, p, m);
        try_shoot(e, m, p, s);
    }
}
