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

static void spawn_dmg_popup(player_t *p, enemy_t *e, int dmg, int head)
{
    popup_t d = {0};
    float dx = e->x - p->x;
    float dy = e->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    float size = 0;

    if (dist < 8.0f)
        dist = 8.0f;
    size = (TILE_SIZE * p->wh) / dist;
    d.amount = dmg;
    d.kind = head ? POP_HEAD : POP_DMG;
    spawn_popup(p, (rel / FOV + 0.5f) * p->ww,
        p->wh / 2.0f + p->pitch - size * 0.55f, d);
}

static void damage_enemy(enemy_t *e, player_t *p)
{
    float scale = e->boss ? BOSS_SCALE : 1.0f;
    int dmg = SHOT_DMG;
    int head = aim_hit(e, p, HEAD_RADIUS * scale
        * (p->aiming ? AIM_HEAD_MULT : 1.0f));

    if (head)
        dmg = HEADSHOT_DMG;
    e->hp -= dmg;
    spawn_dmg_popup(p, e, dmg, head);
    if (e->hp <= 0) {
        e->dying = sfTrue;
        e->death_t = 0;
        add_kill(p, e, head);
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
