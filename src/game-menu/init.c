/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** init.c
*/

#include "macros.h"
#include "wolf.h"
#include "proto.h"

int init_window(game_t *g)
{
    sfVideoMode mode;

    if (g->settings.fullscreen)
        g->window = open_fullscreen();
    if (!g->window) {
        g->settings.fullscreen = sfFalse;
        mode.width = g->settings.win_w;
        mode.height = g->settings.win_h;
        mode.bitsPerPixel = 32;
        g->window = sfRenderWindow_create(mode, TITLE, sfDefaultStyle, NULL);
    }
    if (!g->window) {
        fprintf(stderr, "Error: cannot create window\n");
        return -1;
    }
    sfRenderWindow_setFramerateLimit(g->window, FPS_LIMIT);
    return 0;
}

int game_init(game_t *g)
{
    memset(g, 0, sizeof(game_t));
    g->settings.music_vol = VOL_DEFAULT;
    g->settings.sfx_vol = VOL_DEFAULT;
    g->settings.res_index = RES_DEFAULT;
    g->settings.fullscreen = sfFalse;
    g->settings.win_w = RES_W[RES_DEFAULT];
    g->settings.win_h = RES_H[RES_DEFAULT];
    load_settings(&g->settings);
    g->running = sfTrue;
    g->state = STATE_MENU;
    if (init_window(g) == -1)
        return -1;
    g->font_big = load_font_safe();
    if (!g->font_big) {
        sfRenderWindow_destroy(g->window);
        return -1;
    }
    g->font_med = g->font_big;
    return 0;
}
