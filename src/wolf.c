/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include "proto.h"

static const int RES_W[NUM_RES] = {800, 1024, 1280, 1920};
static const int RES_H[NUM_RES] = {600, 768, 720, 1080};

void get_resolution(int idx, int *w, int *h)
{
    if (idx < 0 || idx >= NUM_RES)
        idx = RES_DEFAULT;
    *w = RES_W[idx];
    *h = RES_H[idx];
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

static int load_settings(settings_t *s)
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
    s->win_w = RES_W[s->res_index];
    s->win_h = RES_H[s->res_index];
    return 0;
}

static void set_vert(sfVertex *v, float x, float y, const sfColor *col)
{
    v->position.x = x;
    v->position.y = y;
    v->color = *col;
    v->texCoords.x = 0;
    v->texCoords.y = 0;
}

sfVertexArray *create_gradient_bg(const sfColor *top, const sfColor *bot,
    float w, float h)
{
    sfVertexArray *va;

    va = sfVertexArray_create();
    if (!va)
        return NULL;
    sfVertexArray_setPrimitiveType(va, sfQuads);
    sfVertexArray_resize(va, 4);
    set_vert(sfVertexArray_getVertex(va, 0), 0, 0, top);
    set_vert(sfVertexArray_getVertex(va, 1), w, 0, top);
    set_vert(sfVertexArray_getVertex(va, 2), w, h, bot);
    set_vert(sfVertexArray_getVertex(va, 3), 0, h, bot);
    return va;
}

sfFont *load_font_safe(void)
{
    sfFont *f;

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

static sfRenderWindow *open_fullscreen(void)
{
    sfVideoMode mode;
    sfRenderWindow *win;

    mode = sfVideoMode_getDesktopMode();
    win = sfRenderWindow_create(mode, TITLE, sfNone, NULL);
    if (win) {
        sfRenderWindow_setPosition(win, (sfVector2i){0, 0});
        sfRenderWindow_requestFocus(win);
    }
    return win;
}

static int init_window(game_t *g)
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

static int game_init(game_t *g)
{
    memset(g, 0, sizeof(game_t));
    g->settings = (settings_t){VOL_DEFAULT, VOL_DEFAULT,
        RES_DEFAULT, sfFalse, RES_W[RES_DEFAULT], RES_H[RES_DEFAULT]};
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

int wolf(void)
{
    game_t g;
    menu_t m;

    if (game_init(&g) == -1)
        return EXIT_FAIL;
    while (g.running && sfRenderWindow_isOpen(g.window)) {
        if (init_menu(&m, &g) == -1)
            break;
        run_menu(&m);
        g.selected_map = m.chosen_map;
        g.running = (m.action != MENU_QUIT);
        cleanup_menu(&m);
    }
    if (g.font_big)
        sfFont_destroy(g.font_big);
    if (g.window)
        sfRenderWindow_destroy(g.window);
    return EXIT_SUCCESS;
}
