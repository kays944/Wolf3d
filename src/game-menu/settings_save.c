/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings_save.c
*/

#include "game_proto.h"

static const int RES_W[NUM_RES] = {800, 1024, 1280, 1920};
static const int RES_H[NUM_RES] = {600, 768, 720, 1080};

void get_resolution(int idx, int *w, int *h)
{
    if (idx < 0 || idx >= NUM_RES)
        idx = RES_DEFAULT;
    *w = RES_W[idx];
    *h = RES_H[idx];
}

void default_settings(settings_t *s)
{
    s->music_vol = VOL_DEFAULT;
    s->sfx_vol = VOL_DEFAULT;
    s->res_index = RES_DEFAULT;
    s->fullscreen = sfFalse;
    get_resolution(RES_DEFAULT, &s->win_w, &s->win_h);
}

void save_settings(settings_t *s)
{
    FILE *f;

    f = fopen(CFG_PATH, "w");
    if (!f)
        return;
    fprintf(f, "music_vol %f\n", s->music_vol);
    fprintf(f, "sfx_vol %f\n", s->sfx_vol);
    fprintf(f, "res_index %d\n", s->res_index);
    fprintf(f, "fullscreen %d\n", s->fullscreen);
    fclose(f);
}

int load_settings(settings_t *s)
{
    FILE *f;

    f = fopen(CFG_PATH, "r");
    if (!f)
        return -1;
    fscanf(f, "music_vol %f\n", &s->music_vol);
    fscanf(f, "sfx_vol %f\n", &s->sfx_vol);
    fscanf(f, "res_index %d\n", &s->res_index);
    fscanf(f, "fullscreen %d\n", (int *)&s->fullscreen);
    fclose(f);
    if (s->res_index < 0 || s->res_index >= NUM_RES)
        s->res_index = RES_DEFAULT;
    get_resolution(s->res_index, &s->win_w, &s->win_h);
    return 0;
}
