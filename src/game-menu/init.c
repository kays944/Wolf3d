/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** init.c
*/

#include "macros.h"
#include "wolf.h"
#include "proto.h"

int load_settings(settings_t *s)
{
    FILE *f = NULL;

    f = fopen(CFG_PATH, "r");
    if (!f)
        return EXIT_FAIL;
    fscanf(f, "music_vol %f\n", &s->music_vol);
    fscanf(f, "sfx_vol %f\n", &s->sfx_vol);
    fscanf(f, "res_index %d\n", &s->res_index);
    fscanf(f, "fullscreen %d\n", (int *)&s->fullscreen);
    fscanf(f, "gamepad %d\n", &s->gamepad);
    fclose(f);
    if (s->res_index < 0 || s->res_index >= NUM_RES)
        s->res_index = RES_DEFAULT;
    s->win_w = RES_W[s->res_index];
    s->win_h = RES_H[s->res_index];
    return EXIT_SUCCESS;
}

sfRenderWindow *open_fullscreen(void)
{
    sfVideoMode mode = sfVideoMode_getDesktopMode();
    sfRenderWindow *win = NULL;
    sfVector2i corner = {0};

    win = sfRenderWindow_create(mode, TITLE, sfNone, NULL);
    if (!win)
        return NULL;
    sfRenderWindow_setPosition(win, corner);
    sfRenderWindow_requestFocus(win);
    return win;
}

int init_window(game_t *g)
{
    sfVideoMode mode = {0};

    if (g->settings.fullscreen)
        g->window = open_fullscreen();
    if (!g->window) {
        g->settings.fullscreen = sfFalse;
        mode.width = g->settings.win_w;
        mode.height = g->settings.win_h;
        mode.bitsPerPixel = BITS_PER_PIXEL;
        g->window = sfRenderWindow_create(mode, TITLE, sfDefaultStyle, NULL);
    }
    if (!g->window) {
        fprintf(stderr, "Error: cannot create window\n");
        return EXIT_FAIL;
    }
    sfRenderWindow_setFramerateLimit(g->window, FPS_LIMIT);
    return EXIT_SUCCESS;
}

static sfFont *load_font_safe(void)
{
    sfFont *f = NULL;

    f = sfFont_createFromFile("assets/fonts/wolf3d.ttf");
    if (f)
        return f;
    f = sfFont_createFromFile(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    if (f)
        return f;
    f = sfFont_createFromFile(
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf");
    if (f)
        return f;
    f = sfFont_createFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf");
    if (f)
        return f;
    fprintf(stderr, "Error: no font found\n");
    return NULL;
}

int game_init(game_t *g)
{
    g->settings.music_vol = VOL_DEFAULT;
    g->settings.sfx_vol = VOL_DEFAULT;
    g->settings.res_index = RES_DEFAULT;
    g->settings.fullscreen = sfFalse;
    g->settings.win_w = RES_W[RES_DEFAULT];
    g->settings.win_h = RES_H[RES_DEFAULT];
    load_settings(&g->settings);
    g->running = sfTrue;
    g->state = STATE_MENU;
    if (init_window(g) == EXIT_FAIL)
        return EXIT_FAIL;
    g->font_big = load_font_safe();
    if (!g->font_big) {
        sfRenderWindow_destroy(g->window);
        return EXIT_FAIL;
    }
    g->font_med = g->font_big;
    return EXIT_SUCCESS;
}
