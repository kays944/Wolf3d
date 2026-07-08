/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** sound.c
*/

#include "macros.h"
#include "proto.h"

static int init_reload_sound(sound_t *s, settings_t *set)
{
    s->reload_buf = sfSoundBuffer_createFromFile(SND_RELOAD);
    s->reload_snd = sfSound_create();
    if (!s->reload_buf || !s->reload_snd)
        return EXIT_FAIL;
    sfSound_setBuffer(s->reload_snd, s->reload_buf);
    sfSound_setRelativeToListener(s->reload_snd, sfTrue);
    sfSound_setVolume(s->reload_snd, set->sfx_vol);
    return EXIT_SUCCESS;
}

static int init_shoot_sounds(sound_t *s, settings_t *set)
{
    s->shoot_buf = sfSoundBuffer_createFromFile(SND_SHOOT);
    s->shoot_snd = sfSound_create();
    if (!s->shoot_buf || !s->shoot_snd)
        return EXIT_FAIL;
    sfSound_setBuffer(s->shoot_snd, s->shoot_buf);
    sfSound_setRelativeToListener(s->shoot_snd, sfTrue);
    sfSound_setVolume(s->shoot_snd, set->sfx_vol);
    s->pistol_buf = sfSoundBuffer_createFromFile(SND_PISTOL);
    s->pistol_snd = sfSound_create();
    if (!s->pistol_buf || !s->pistol_snd)
        return EXIT_FAIL;
    sfSound_setBuffer(s->pistol_snd, s->pistol_buf);
    sfSound_setRelativeToListener(s->pistol_snd, sfTrue);
    sfSound_setVolume(s->pistol_snd, set->sfx_vol);
    return init_reload_sound(s, set);
}

static int init_musics(sound_t *s)
{
    s->menu_music = sfMusic_createFromFile(SND_MENU);
    s->night_amb = sfMusic_createFromFile(SND_NIGHT_AMB);
    s->day_amb = sfMusic_createFromFile(SND_DAY_AMB);
    if (!s->menu_music || !s->night_amb || !s->day_amb)
        return EXIT_FAIL;
    sfMusic_setLoop(s->menu_music, sfTrue);
    sfMusic_setLoop(s->night_amb, sfTrue);
    sfMusic_setLoop(s->day_amb, sfTrue);
    sfMusic_setVolume(s->night_amb, 0);
    sfMusic_setVolume(s->day_amb, 0);
    return EXIT_SUCCESS;
}

int init_sound(sound_t *s, settings_t *set)
{
    if (init_musics(s) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_shoot_sounds(s, set) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_fx(s) == EXIT_FAIL)
        return EXIT_FAIL;
    s->sfx_vol = set->sfx_vol;
    s->music_vol = set->music_vol;
    sfMusic_setVolume(s->menu_music, set->music_vol);
    return EXIT_SUCCESS;
}

void destroy_sound(sound_t *s)
{
    destroy_fx(s);
    if (s->reload_snd)
        sfSound_destroy(s->reload_snd);
    if (s->reload_buf)
        sfSoundBuffer_destroy(s->reload_buf);
    if (s->pistol_snd)
        sfSound_destroy(s->pistol_snd);
    if (s->pistol_buf)
        sfSoundBuffer_destroy(s->pistol_buf);
    if (s->shoot_snd)
        sfSound_destroy(s->shoot_snd);
    if (s->shoot_buf)
        sfSoundBuffer_destroy(s->shoot_buf);
    if (s->menu_music)
        sfMusic_destroy(s->menu_music);
    if (s->night_amb)
        sfMusic_destroy(s->night_amb);
    if (s->day_amb)
        sfMusic_destroy(s->day_amb);
}

void update_sound_vol(sound_t *s, settings_t *set)
{
    s->sfx_vol = set->sfx_vol;
    s->music_vol = set->music_vol;
    sfMusic_setVolume(s->menu_music, set->music_vol);
    sfSound_setVolume(s->shoot_snd, set->sfx_vol);
    sfSound_setVolume(s->reload_snd, set->sfx_vol);
    sfSound_setVolume(s->pistol_snd, set->sfx_vol);
}

void play_shoot(sound_t *s)
{
    sfSound_play(s->shoot_snd);
}

void play_reload(sound_t *s)
{
    sfSound_play(s->reload_snd);
}

void play_pistol(sound_t *s)
{
    sfSound_play(s->pistol_snd);
}
