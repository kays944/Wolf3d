/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** boss.c — boss moveset: fan shot, charge, ground slam, enrage, summons
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static float bdist(enemy_t *e, player_t *p)
{
    float dx = p->x - e->x;
    float dy = p->y - e->y;

    return sqrtf(dx * dx + dy * dy);
}

void sync_boss_state(enemy_t *e)
{
    if (!e->boss)
        return;
    e->enraged = (e->hp * 2 < e->max_hp) ? sfTrue : sfFalse;
    e->summons = (e->hp * 3 < e->max_hp * 2) + (e->hp * 3 < e->max_hp);
}

static void summon_wave(enemy_t *e, map_t *m)
{
    static const int off[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int done = 0;
    float x = 0;
    float y = 0;

    for (int i = 0; i < 4 && done < BOSS_SUMMON_N; i++) {
        x = e->x + off[i][0] * TILE_SIZE;
        y = e->y + off[i][1] * TILE_SIZE;
        if (is_blocked(x, y, m) != IS_WALL) {
            spawn_enemy(m, x, y, ENEMY_TYPE_GRUNT);
            done++;
        }
    }
}

static void check_phase(enemy_t *e, map_t *m, sound_t *s)
{
    int want = (e->hp * 3 < e->max_hp * 2) + (e->hp * 3 < e->max_hp);

    if (!e->enraged && e->hp * 2 < e->max_hp) {
        e->enraged = sfTrue;
        play_fx_at(s, FX_BOSS, e->x, e->y);
    }
    while (e->summons < want) {
        summon_wave(e, m);
        e->summons++;
        play_fx_at(s, FX_BOSS, e->x, e->y);
    }
}

static void boss_slam(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    e->cooldown = BOSS_SLAM_CD * (e->enraged ? BOSS_ENRAGE_CD_MULT : 1.0f);
    e->atk_anim = ATK_ANIM_LEN;
    spawn_boom(m, e->x, e->y);
    play_fx_at(s, FX_BOOM, e->x, e->y);
    if (bdist(e, p) < BOSS_SLAM_RADIUS && p->z < PROJ_DODGE_Z)
        hurt_player(p, BOSS_SLAM_DMG);
}

static void charge_step(enemy_t *e, player_t *p, map_t *m)
{
    float d = bdist(e, p);
    float step = BOSS_CHARGE_SPEED * p->dt;
    float nx = 0;
    float ny = 0;

    if (d < 1.0f)
        return;
    nx = e->x + (p->x - e->x) / d * step;
    ny = e->y + (p->y - e->y) / d * step;
    if (is_blocked(nx, ny, m) != IS_WALL && !foe_overlap(m, e, nx, ny)) {
        e->x = nx;
        e->y = ny;
        e->moving = sfTrue;
        return;
    }
    e->charge_t = 0;
    e->cooldown = BOSS_CHARGE_CD * (e->enraged ? BOSS_ENRAGE_CD_MULT : 1.0f);
}

static void boss_charge(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    e->charge_t -= p->dt;
    if (bdist(e, p) < BOSS_SLAM_RANGE || e->charge_t <= 0) {
        e->charge_t = 0;
        boss_slam(e, p, m, s);
        return;
    }
    charge_step(e, p, m);
}

static void start_charge(enemy_t *e, sound_t *s)
{
    e->charge_t = BOSS_CHARGE_LEN;
    play_fx_at(s, FX_BOSS, e->x, e->y);
}

static void boss_shoot(enemy_t *e, map_t *m, player_t *p, sound_t *s)
{
    float base = atan2f(p->y - e->y, p->x - e->x);
    int n = e->enraged ? 2 : 1;

    e->cooldown = BOSS_SHOOT_CD * (e->enraged ? BOSS_ENRAGE_CD_MULT : 1.0f);
    e->atk_anim = ATK_ANIM_LEN;
    play_fx_at(s, FX_ESHOT, e->x, e->y);
    for (int k = -n; k <= n; k++)
        spawn_proj_angle(m, e, base + k * BOSS_SPREAD);
}

void tick_boss(enemy_t *e, player_t *p, map_t *m, sound_t *s)
{
    float d = bdist(e, p);

    check_phase(e, m, s);
    if (e->charge_t > 0) {
        boss_charge(e, p, m, s);
        return;
    }
    chase(e, p, m);
    if (e->cooldown > 0)
        return;
    if (d < BOSS_SLAM_RANGE) {
        boss_slam(e, p, m, s);
        return;
    }
    if (d >= BOSS_CHARGE_MIN && d < BOSS_CHARGE_MAX) {
        start_charge(e, s);
        return;
    }
    if (m->night && !p->flashlight && d > NIGHT_SIGHT)
        return;
    if (d <= ENEMY_SHOOT_RANGE)
        boss_shoot(e, m, p, s);
}
