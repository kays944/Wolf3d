/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** explosion.c — exploding barrels: blast damage, chain reaction, booms
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void spawn_boom(map_t *m, float x, float y)
{
    for (int i = 0; i < MAX_BOOMS; i++) {
        if (m->booms[i].active)
            continue;
        m->booms[i].x = x;
        m->booms[i].y = y;
        m->booms[i].t = 0;
        m->booms[i].active = sfTrue;
        return;
    }
}

static int in_blast(map_t *m, prop_t *from, float tx, float ty)
{
    float dx = tx - from->x;
    float dy = ty - from->y;
    float d = sqrtf(dx * dx + dy * dy);
    float x = from->x;
    float y = from->y;

    if (d > BOOM_RADIUS)
        return 0;
    if (d < 1.0f)
        return 1;
    dx = dx / d * LOS_STEP;
    dy = dy / d * LOS_STEP;
    for (float t = 0; t < d; t += LOS_STEP) {
        if (is_wall(x, y, m) == IS_WALL)
            return 0;
        x += dx;
        y += dy;
    }
    return 1;
}

static void damage_foe(player_t *p, enemy_t *e)
{
    e->hp -= BOOM_DMG;
    if (e->hp > 0)
        return;
    e->dying = sfTrue;
    e->death_t = 0;
    add_kill(p, e, 0);
}

static void hurt_foes(player_t *p, map_t *m, prop_t *from)
{
    enemy_t *e = NULL;

    if (in_blast(m, from, p->x, p->y))
        hurt_player(p, BOOM_PLAYER_DMG);
    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive || e->dying)
            continue;
        if (in_blast(m, from, e->x, e->y))
            damage_foe(p, e);
    }
}

static void light_chain(map_t *m, prop_t *from)
{
    prop_t *o = NULL;
    float d = 0;

    for (int i = 0; i < m->prop_count; i++) {
        o = &m->props[i];
        if (o == from || o->type != PROP_BARREL || o->dead || o->fuse > 0)
            continue;
        if (!in_blast(m, from, o->x, o->y))
            continue;
        d = hypotf(o->x - from->x, o->y - from->y);
        o->fuse = BOOM_CHAIN_MIN + BOOM_CHAIN_VAR * d / BOOM_RADIUS;
    }
}

static void explode_barrel(player_t *p, map_t *m, sound_t *s, prop_t *pr)
{
    sfSound *snd = NULL;

    kill_prop(m, pr);
    spawn_boom(m, pr->x, pr->y);
    snd = play_fx_at(s, FX_BOOM, pr->x, pr->y);
    if (snd)
        sfSound_setMinDistance(snd, BOOM_SND_DIST);
    hurt_foes(p, m, pr);
    light_chain(m, pr);
}

static int aim_barrel(prop_t *pr, player_t *p)
{
    float dx = pr->x - p->x;
    float dy = pr->y - p->y;
    float dist = sqrtf(dx * dx + dy * dy);
    float rel = norm_angle(atan2f(dy, dx) - p->angle);

    if (dist > SHOT_RANGE)
        return 0;
    return fabsf(rel) < atan2f(BARREL_RADIUS, dist);
}

void shoot_barrels(player_t *p, map_t *m, sound_t *s)
{
    prop_t *pr = NULL;
    prop_t *best = NULL;
    float bd = 0;
    float d = 0;

    for (int i = 0; i < m->prop_count; i++) {
        pr = &m->props[i];
        if (pr->type != PROP_BARREL || pr->dead)
            continue;
        if (!aim_barrel(pr, p) || !has_los(pr->x, pr->y, p, m))
            continue;
        d = hypotf(pr->x - p->x, pr->y - p->y);
        if (!best || d < bd) {
            bd = d;
            best = pr;
        }
    }
    if (best)
        explode_barrel(p, m, s, best);
}

static void tick_fuses(player_t *p, map_t *m, sound_t *s)
{
    prop_t *pr = NULL;

    for (int i = 0; i < m->prop_count; i++) {
        pr = &m->props[i];
        if (pr->type != PROP_BARREL || pr->dead || pr->fuse <= 0)
            continue;
        pr->fuse -= p->dt;
        if (pr->fuse <= 0)
            explode_barrel(p, m, s, pr);
    }
}

void update_booms(player_t *p, map_t *m, sound_t *s)
{
    for (int i = 0; i < MAX_BOOMS; i++) {
        if (!m->booms[i].active)
            continue;
        m->booms[i].t += p->dt;
        if (m->booms[i].t >= BOOM_FRAMES * BOOM_FRAME_LEN)
            m->booms[i].active = sfFalse;
    }
    tick_fuses(p, m, s);
}
