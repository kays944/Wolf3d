/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** sound_amb.c — listener tracking, footsteps, damage/pickup cues,
** night ambience fade + nightfall howl
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void update_listener(player_t *p)
{
    sfListener_setPosition((sfVector3f){p->x, 0, p->y});
    sfListener_setDirection((sfVector3f){cosf(p->angle), 0, sinf(p->angle)});
}

static void update_steps(player_t *p, sound_t *s)
{
    float dx = p->x - s->last_px;
    float dy = p->y - s->last_py;
    float d = sqrtf(dx * dx + dy * dy);

    s->last_px = p->x;
    s->last_py = p->y;
    if (d > TILE_SIZE) {
        s->step_acc = 0;
        return;
    }
    if (p->z > 0.5f)
        return;
    s->step_acc += d;
    if (s->step_acc >= STEP_DIST) {
        s->step_acc = 0;
        play_fx(s, FX_STEP);
    }
}

static void watch_player(player_t *p, sound_t *s)
{
    if (s->last_hp < 0) {
        s->last_hp = p->hp;
        s->last_res = p->reserve;
    }
    if (p->hp < s->last_hp)
        play_fx(s, FX_HURT);
    if (p->hp > s->last_hp || p->reserve > s->last_res)
        play_fx(s, FX_PICKUP);
    s->last_hp = p->hp;
    s->last_res = p->reserve;
}

static void update_night_amb(map_t *m, sound_t *s, float dt)
{
    float target = m->night ? s->music_vol * NIGHT_AMB_VOL : 0;

    if (s->last_nights >= 0 && m->nights > s->last_nights)
        play_fx(s, FX_HOWL);
    s->last_nights = m->nights;
    if (s->amb_vol < target)
        s->amb_vol += AMB_FADE_RATE * dt;
    if (s->amb_vol > target)
        s->amb_vol -= AMB_FADE_RATE * dt;
    if (s->amb_vol < 0)
        s->amb_vol = 0;
    if (!s->night_amb)
        return;
    sfMusic_setVolume(s->night_amb, s->amb_vol);
    if (s->amb_vol > 0.5f && sfMusic_getStatus(s->night_amb) != sfPlaying)
        sfMusic_play(s->night_amb);
    if (s->amb_vol <= 0.5f && sfMusic_getStatus(s->night_amb) == sfPlaying)
        sfMusic_pause(s->night_amb);
}

void reset_ambience(sound_t *s)
{
    s->last_hp = -1;
    s->last_nights = -1;
    s->step_acc = 0;
    s->amb_vol = 0;
    if (s->night_amb) {
        sfMusic_stop(s->night_amb);
        sfMusic_setVolume(s->night_amb, 0);
    }
}

void update_ambience(player_t *p, map_t *m, sound_t *s)
{
    update_listener(p);
    update_steps(p, s);
    watch_player(p, s);
    update_night_amb(m, s, p->dt);
}
