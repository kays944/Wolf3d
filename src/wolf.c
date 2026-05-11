/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include <stdio.h>
#include <math.h>
#include "macros.h"
#include "wolf.h"
#include "proto.h"

const int RES_W[NUM_RES] = {800, 1024, 1280, 1920};
const int RES_H[NUM_RES] = {600, 768, 720, 1080};

static sfRenderWindow *creation_window(void)
{
    sfVideoMode mode = {WIN_WIDTH, WIN_HEIGHT, FREQUENCY};
    sfRenderWindow *window = {0};

    window = sfRenderWindow_create(mode, "fen1", sfResize | sfClose, NULL);
    return window;
}

static int init_player(char **map, player_t *player)
{
    int col = 0;
    int row = 0;
    int find = 0;

    for (; col != MAP_HEIGHT; ++row) {
        if (map[col][row] == 'o') {
            find = 1;
            break;
        }
        if (row == MAP_WIDTH) {
            col += 1;
            row = -1;
        }
    }
    if (find == 0)
        return EXIT_FAIL;
    player->x = row * TILE_SIZE + TILE_SIZE / 2;
    player->y = col * TILE_SIZE + TILE_SIZE / 2;
    player->angle = fmod(0, 2 * M_PI);
    return EXIT_SUCCESS;
}

static int game_loop(char **map, game_t *g)
{
    sfRenderWindow *window = creation_window();
    player_t player = {0};

    if (!window || init_player(map, &player) == EXIT_FAIL)
        return EXIT_FAIL;
    while (sfRenderWindow_isOpen(window)) {
        if (event(window, &player, map) == EVENT_CLOSE)
            break;
        draw(window, &player, map);
    }
    close_all(window);
    return EXIT_SUCCESS;
}

static void set_vert(sfVertex *v, float x, float y, const sfColor *col)
{
    v->position.x = x;
    v->position.y = y;
    v->color = *col;
    v->texCoords.x = 0;
    v->texCoords.y = 0;
}

void get_resolution(int idx, int *w, int *h)
{
    if (idx < 0 || idx >= NUM_RES)
        idx = RES_DEFAULT;
    *w = RES_W[idx];
    *h = RES_H[idx];
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

sfRenderWindow *open_fullscreen(void)
{
    sfVideoMode mode;
    sfRenderWindow *win;
    sfVector2i corner;

    mode = sfVideoMode_getDesktopMode();
    win = sfRenderWindow_create(mode, TITLE, sfNone, NULL);
    if (!win)
        return NULL;
    corner.x = 0;
    corner.y = 0;
    sfRenderWindow_setPosition(win, corner);
    sfRenderWindow_requestFocus(win);
    return win;
}

int wolf(void)
{
    char **map = parsing_map(BASIC_MAP_PATH);
    game_t g = {0};
    menu_t m = {0};

    if (!map || game_init(&g) == -1)
        return EXIT_FAIL;
    while (g.running && sfRenderWindow_isOpen(g.window)) {
        if (init_menu(&m, &g) == -1)
            break;
        run_menu(&m);
        g.selected_map = m.chosen_map;
        g.running = (m.action != MENU_QUIT);
        cleanup_menu(&m);
    }
    game_loop(map, &g);
    if (g.font_big)
        sfFont_destroy(g.font_big);
    if (g.window)
        sfRenderWindow_destroy(g.window);
    return EXIT_SUCCESS;
}
