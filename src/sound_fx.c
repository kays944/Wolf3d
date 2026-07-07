/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** sound_fx.c — one-shot sound pool, spatialized or listener-relative
*/

#include <stdlib.h>
#include "macros.h"
#include "proto.h"

static int load_fx_bufs(sound_t *s)
{
    static const char *const paths[FX_KINDS] = {SND_STEP, SND_STEP2,
        SND_STEP3, SND_GROWL, SND_BOSS, SND_HOWL, SND_HURT, SND_PICKUP,
        SND_DOOR, SND_ESHOT, SND_BITE, SND_BOOM};

    for (int i = 0; i < FX_KINDS; i++) {
        s->fx_bufs[i] = sfSoundBuffer_createFromFile(paths[i]);
        if (!s->fx_bufs[i])
            return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static void set_fx_params(sound_t *s)
{
    static const float vol[FX_KINDS] = {
        0.75f, 0.75f, 0.75f, 1.0f, 1.0f, 0.9f, 1.0f, 0.8f, 0.9f, 1.0f,
        1.0f, 1.0f};
    static const float jit[FX_KINDS] = {
        0.08f, 0.08f, 0.08f, 0.10f, 0.05f, 0.03f, 0.08f, 0.0f, 0.04f,
        0.06f, 0.12f, 0.06f};

    for (int i = 0; i < FX_KINDS; i++) {
        s->fx_vol[i] = vol[i];
        s->fx_pitch[i] = 1.0f;
        s->fx_jit[i] = jit[i];
    }
}

int init_fx(sound_t *s)
{
    if (load_fx_bufs(s) == EXIT_FAIL)
        return EXIT_FAIL;
    set_fx_params(s);
    for (int i = 0; i < SND_POOL; i++) {
        s->pool[i] = sfSound_create();
        if (!s->pool[i])
            return EXIT_FAIL;
        sfSound_setMinDistance(s->pool[i], SND_MIN_DIST);
        sfSound_setAttenuation(s->pool[i], SND_ATTENUATION);
    }
    s->last_hp = -1;
    s->last_nights = -1;
    return EXIT_SUCCESS;
}

void destroy_fx(sound_t *s)
{
    for (int i = 0; i < SND_POOL; i++) {
        if (s->pool[i])
            sfSound_destroy(s->pool[i]);
        s->pool[i] = NULL;
    }
    for (int i = 0; i < FX_KINDS; i++) {
        if (s->fx_bufs[i])
            sfSoundBuffer_destroy(s->fx_bufs[i]);
        s->fx_bufs[i] = NULL;
    }
}

static sfSound *grab(sound_t *s, int id)
{
    sfSound *snd = s->pool[s->pool_i];
    float jit = 1.0f + s->fx_jit[id] * ((rand() % 201) - 100) / 100.0f;

    if (!snd || !s->fx_bufs[id])
        return NULL;
    s->pool_i = (s->pool_i + 1) % SND_POOL;
    sfSound_setBuffer(snd, s->fx_bufs[id]);
    sfSound_setVolume(snd, s->sfx_vol * s->fx_vol[id]);
    sfSound_setPitch(snd, s->fx_pitch[id] * jit);
    return snd;
}

void play_fx(sound_t *s, int id)
{
    sfSound *snd = grab(s, id);

    if (!snd)
        return;
    sfSound_setRelativeToListener(snd, sfTrue);
    sfSound_setPosition(snd, (sfVector3f){0, 0, 0});
    sfSound_play(snd);
}

sfSound *play_fx_at(sound_t *s, int id, float x, float y)
{
    sfSound *snd = grab(s, id);

    if (!snd)
        return NULL;
    sfSound_setRelativeToListener(snd, sfFalse);
    sfSound_setPosition(snd, (sfVector3f){x, 0, y});
    sfSound_play(snd);
    return snd;
}
