/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_melee.c — close combat (runner bite) and body blocking
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static float dist_to(enemy_t *e, player_t *p)
{
    float dx = p->x - e->x;
    float dy = p->y - e->y;

    return sqrtf(dx * dx + dy * dy);
}

int foe_overlap(map_t *m, enemy_t *self, float nx, float ny)
{
    enemy_t *o = NULL;
    float r = 0;
    float d2 = 0;
    float od2 = 0;

    for (int i = 0; i < m->enemy_count; i++) {
        o = &m->enemies[i];
        if (o == self || !o->alive || o->dying)
            continue;
        r = ENEMY_RADIUS * (o->boss ? BOSS_SCALE : 1.0f)
            + ENEMY_RADIUS * (self->boss ? BOSS_SCALE : 1.0f);
        d2 = (o->x - nx) * (o->x - nx) + (o->y - ny) * (o->y - ny);
        od2 = (o->x - self->x) * (o->x - self->x)
            + (o->y - self->y) * (o->y - self->y);
        if (d2 < r * r && d2 < od2)
            return 1;
    }
    return 0;
}

void hurt_player(player_t *p, int dmg)
{
    if (p->hurt_cd > 0)
        return;
    p->hurt_cd = HURT_COOLDOWN;
    p->hp -= (int)(dmg * diff_dmg_mult(p->difficulty) + 0.5f);
    p->hurt_flash = HURT_FLASH_FRAMES;
    set_health_frame(p);
}

void play_death_cry(enemy_t *e, sound_t *s)
{
    int id = FX_DIE_GRUNT;

    if (e->type == ENEMY_TYPE_RUNNER)
        id = FX_DIE_RUNNER;
    if (e->boss)
        id = FX_DIE_BOSS;
    play_fx_at(s, id, e->x, e->y);
}

void try_bite(enemy_t *e, player_t *p, sound_t *s)
{
    if (dist_to(e, p) > RUNNER_MELEE_RANGE || e->cooldown > 0)
        return;
    if (p->z > PROJ_DODGE_Z)
        return;
    e->cooldown = RUNNER_MELEE_CD;
    e->atk_anim = ATK_ANIM_LEN;
    play_fx_at(s, FX_BITE, e->x, e->y);
    hurt_player(p, RUNNER_MELEE_DMG);
}
