/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** sound_amb.c — listener tracking, footsteps, damage/pickup cues,
** night ambience fade + nightfall howl
*/

#include <math.h>
#include <stdlib.h>
#include "macros.h"
#include "proto.h"

static int next_step(sound_t *s)
{
    int id = FX_STEP + rand() % 3;

    if (id == s->last_step)
        id = FX_STEP + (id - FX_STEP + 1) % 3;
    s->last_step = id;
    return id;
}

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
        play_fx(s, next_step(s));
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

static float approach(float v, float target, float dt)
{
    if (v < target)
        v += AMB_FADE_RATE * dt;
    if (v > target)
        v -= AMB_FADE_RATE * dt;
    return v < 0 ? 0 : v;
}

static void set_amb_track(sfMusic *mus, float vol)
{
    if (!mus)
        return;
    sfMusic_setVolume(mus, vol);
    if (vol > 0.5f && sfMusic_getStatus(mus) != sfPlaying)
        sfMusic_play(mus);
    if (vol <= 0.5f && sfMusic_getStatus(mus) == sfPlaying)
        sfMusic_pause(mus);
}

static void update_night_amb(map_t *m, sound_t *s, float dt)
{
    float night_t = m->night ? s->music_vol * NIGHT_AMB_VOL : 0;
    float day_t = m->night ? 0 : s->music_vol * DAY_AMB_VOL;

    if (s->last_nights >= 0 && m->nights > s->last_nights)
        play_fx(s, FX_HOWL);
    s->last_nights = m->nights;
    s->amb_vol = approach(s->amb_vol, night_t, dt);
    s->day_vol = approach(s->day_vol, day_t, dt);
    set_amb_track(s->night_amb, s->amb_vol);
    set_amb_track(s->day_amb, s->day_vol);
}

static void update_night_fx(player_t *p, map_t *m, sound_t *s)
{
    float a = 0;
    float d = 0;

    if (!m->night) {
        if (s->night_fx_cd < NIGHT_FX_MIN)
            s->night_fx_cd = NIGHT_FX_MIN;
        return;
    }
    s->night_fx_cd -= p->dt;
    if (s->night_fx_cd > 0)
        return;
    s->night_fx_cd = NIGHT_FX_MIN + rand() % NIGHT_FX_VAR;
    a = (rand() % 628) / 100.0f;
    d = NIGHT_FX_DIST_MIN + rand() % NIGHT_FX_DIST_VAR;
    play_fx_at(s, FX_NIGHT1 + rand() % 3,
        p->x + cosf(a) * d, p->y + sinf(a) * d);
}

void reset_ambience(sound_t *s)
{
    s->last_hp = -1;
    s->last_nights = -1;
    s->step_acc = 0;
    s->night_fx_cd = NIGHT_FX_MIN;
    s->amb_vol = 0;
    s->day_vol = 0;
    if (s->night_amb) {
        sfMusic_stop(s->night_amb);
        sfMusic_setVolume(s->night_amb, 0);
    }
    if (s->day_amb) {
        sfMusic_stop(s->day_amb);
        sfMusic_setVolume(s->day_amb, 0);
    }
}

void update_ambience(player_t *p, map_t *m, sound_t *s)
{
    update_listener(p);
    update_steps(p, s);
    watch_player(p, s);
    update_night_amb(m, s, p->dt);
    update_night_fx(p, m, s);
}
