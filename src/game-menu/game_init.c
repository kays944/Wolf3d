/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_init.c
*/

#include "game_proto.h"

static sfRenderWindow *create_fullscreen(void)
{
    sfVideoMode mode = {0};
    sfRenderWindow *win = {0};

    mode = sfVideoMode_getDesktopMode();
    win = sfRenderWindow_create(mode, TITLE, sfNone, NULL);
    if (win) {
        sfRenderWindow_setPosition(win, (sfVector2i){0, 0});
        sfRenderWindow_requestFocus(win);
    }
    return win;
}

static sfRenderWindow *create_windowed(game_t *g)
{
    sfVideoMode mode = {0};

    mode.width = g->settings.win_w;
    mode.height = g->settings.win_h;
    mode.bitsPerPixel = BITS_PER_PIXEL;
    return sfRenderWindow_create(mode, TITLE, sfDefaultStyle, NULL);
}

static int open_window(game_t *g)
{
    if (g->settings.fullscreen)
        g->window = create_fullscreen();
    if (!g->window) {
        g->settings.fullscreen = sfFalse;
        g->window = create_windowed(g);
    }
    if (!g->window) {
        fprintf(stderr, "Error: cannot create window\n");
        return EXIT_FAIL;
    }
    sfRenderWindow_setFramerateLimit(g->window, FPS_LIMIT);
    return EXIT_SUCCESS;
}

static int load_fonts(game_t *g)
{
    g->font_big = load_font_safe();
    if (!g->font_big) {
        fprintf(stderr, "Error: no usable font found\n");
        return EXIT_FAIL;
    }
    g->font_med = g->font_big;
    return EXIT_SUCCESS;
}

int game_init(game_t *g)
{
    memset(g, 0, sizeof(game_t));
    default_settings(&g->settings);
    load_settings(&g->settings);
    g->running = sfTrue;
    g->state = STATE_MENU;
    if (open_window(g) == EXIT_FAIL)
        return EXIT_FAIL;
    if (load_fonts(g) == EXIT_FAIL) {
        sfRenderWindow_destroy(g->window);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}
