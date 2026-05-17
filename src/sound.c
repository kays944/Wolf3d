/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** sound.c
*/

#include "macros.h"
#include "proto.h"

int init_sound(sound_t *s, settings_t *set)
{
    s->menu_music = sfMusic_createFromFile(SND_MENU);
    s->game_music = sfMusic_createFromFile(SND_GAME);
    if (!s->menu_music || !s->game_music)
        return EXIT_FAIL;
    sfMusic_setLoop(s->menu_music, sfTrue);
    sfMusic_setLoop(s->game_music, sfTrue);
    s->shoot_buf = sfSoundBuffer_createFromFile(SND_SHOOT);
    s->shoot_snd = sfSound_create();
    if (!s->shoot_buf || !s->shoot_snd)
        return EXIT_FAIL;
    sfSound_setBuffer(s->shoot_snd, s->shoot_buf);
    sfMusic_setVolume(s->menu_music, set->music_vol);
    sfMusic_setVolume(s->game_music, set->music_vol);
    sfSound_setVolume(s->shoot_snd, set->sfx_vol);
    return EXIT_SUCCESS;
}

void destroy_sound(sound_t *s)
{
    if (s->shoot_snd)
        sfSound_destroy(s->shoot_snd);
    if (s->shoot_buf)
        sfSoundBuffer_destroy(s->shoot_buf);
    if (s->menu_music)
        sfMusic_destroy(s->menu_music);
    if (s->game_music)
        sfMusic_destroy(s->game_music);
}

void update_sound_vol(sound_t *s, settings_t *set)
{
    sfMusic_setVolume(s->menu_music, set->music_vol);
    sfMusic_setVolume(s->game_music, set->music_vol);
    sfSound_setVolume(s->shoot_snd, set->sfx_vol);
}

void play_shoot(sound_t *s)
{
    sfSound_play(s->shoot_snd);
}
