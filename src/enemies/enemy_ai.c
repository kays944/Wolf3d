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

static float vision_range(player_t *p, map_t *m, float day_range)
{
    if (m->night && !p->flashlight)
        return NIGHT_SIGHT;
    return day_range;
}

static float enemy_speed(enemy_t *e)
{
    if (e->type == ENEMY_TYPE_BRUTE)
        return BRUTE_SPEED;
    if (e->type == ENEMY_TYPE_RUNNER)
        return RUNNER_SPEED;
    return ENEMY_SPEED;
}

static int can_move(enemy_t *e, map_t *m, float x, float y)
{
    return is_blocked(x, y, m) != IS_WALL && !foe_overlap(m, e, x, y);
}

static void step_to(enemy_t *e, map_t *m, float nx, float ny)
{
    if (can_move(e, m, nx, ny)) {
        e->x = nx;
        e->y = ny;
        return;
    }
    if (can_move(e, m, nx, e->y)) {
        e->x = nx;
        return;
    }
    if (can_move(e, m, e->x, ny))
        e->y = ny;
}

static void chase(enemy_t *e, player_t *p, map_t *m)
{
    float dist = dist_to_player(e, p);
    float step = enemy_speed(e) * p->dt;
    float stop = e->type == ENEMY_TYPE_RUNNER
        ? RUNNER_STOP_DIST : ENEMY_STOP_DIST;

    if (dist <= stop || dist > vision_range(p, m, ENEMY_SIGHT))
        return;
    e->moving = sfTrue;
    step_to(e, m, e->x + (p->x - e->x) / dist * step,
        e->y + (p->y - e->y) / dist * step);
}

static void try_shoot(enemy_t *e, map_t *m, player_t *p, sound_t *s)
{
    float dist = dist_to_player(e, p);

    if (dist > vision_range(p, m, ENEMY_SHOOT_RANGE) || e->cooldown > 0)
        return;
    e->cooldown = ENEMY_SHOOT_CD;
    e->atk_anim = ATK_ANIM_LEN;
    play_fx_at(s, FX_ESHOT, e->x, e->y);
    spawn_proj(m, e, p);
}

static void update_growl(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    if (dist_to_player(e, p) > vision_range(p, m, ENEMY_SIGHT)) {
        e->aware = sfFalse;
        return;
    }
    if (e->aware || e->growl_cd > 0) {
        e->aware = sfTrue;
        return;
    }
    e->aware = sfTrue;
    e->growl_cd = GROWL_CD;
    play_fx_at(s, e->boss ? FX_BOSS : FX_GROWL, e->x, e->y);
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
    if (e->type == ENEMY_TYPE_RUNNER)
        try_bite(e, p, s);
    else
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
            if (e->death_t == 0)
                play_death_cry(e, s);
            e->death_t += p->dt;
            continue;
        }
        tick_enemy(e, p, m, s);
    }
}
