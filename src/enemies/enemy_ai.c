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

static float sight_range(player_t *p, map_t *m)
{
    if (m->night && !p->flashlight)
        return NIGHT_SIGHT;
    return ENEMY_SIGHT;
}

static float fire_range(player_t *p, map_t *m)
{
    if (m->night && !p->flashlight)
        return NIGHT_SIGHT;
    return ENEMY_SHOOT_RANGE;
}

static float enemy_speed(enemy_t *e)
{
    if (e->type == ENEMY_TYPE_BRUTE)
        return BRUTE_SPEED;
    if (e->type == ENEMY_TYPE_RUNNER)
        return RUNNER_SPEED;
    return ENEMY_SPEED;
}

static void chase(enemy_t *e, player_t *p, map_t *m)
{
    float dist = dist_to_player(e, p);
    float step = enemy_speed(e) * p->dt;
    float nx = 0;
    float ny = 0;

    if (dist <= ENEMY_STOP_DIST || dist > sight_range(p, m))
        return;
    nx = e->x + (p->x - e->x) / dist * step;
    ny = e->y + (p->y - e->y) / dist * step;
    e->moving = sfTrue;
    if (is_blocked(nx, ny, m) != IS_WALL) {
        e->x = nx;
        e->y = ny;
        return;
    }
    if (is_blocked(nx, e->y, m) != IS_WALL) {
        e->x = nx;
        return;
    }
    if (is_blocked(e->x, ny, m) != IS_WALL)
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

    if (dist > fire_range(p, m) || e->cooldown > 0)
        return;
    e->cooldown = ENEMY_SHOOT_CD;
    e->atk_anim = ATK_ANIM_LEN;
    play_fx_at(s, FX_ESHOT, e->x, e->y);
    spawn_proj(m, e, p);
}

static void update_growl(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    sfSound *snd = NULL;

    if (dist_to_player(e, p) > sight_range(p, m)) {
        e->aware = sfFalse;
        return;
    }
    if (e->aware || e->growl_cd > 0) {
        e->aware = sfTrue;
        return;
    }
    e->aware = sfTrue;
    e->growl_cd = GROWL_CD;
    snd = play_fx_at(s, FX_GROWL, e->x, e->y);
    if (snd && e->boss)
        sfSound_setPitch(snd, BOSS_GROWL_PITCH);
}

static void tick_enemy(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    e->anim_t += p->dt;
    e->moving = sfFalse;
    if (e->cooldown > 0)
        e->cooldown -= p->dt;
    if (e->atk_anim > 0)
        e->atk_anim -= p->dt;
    if (e->growl_cd > 0)
        e->growl_cd -= p->dt;
    if (!has_los(e->x, e->y, p, m)) {
        e->aware = sfFalse;
        return;
    }
    update_growl(e, p, m, s);
    chase(e, p, m);
    try_shoot(e, m, p, s);
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
        tick_enemy(e, p, m, s);
    }
}
