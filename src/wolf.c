/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include <stdio.h>
#include "macros.h"
#include "wolf.h"
#include "proto.h"

const int RES_W[NUM_RES] = {800, 1024, 1280, 1920};
const int RES_H[NUM_RES] = {600, 768, 720, 1080};

static void set_vert(sfVertex *v, float x, float y, const sfColor *col)
{
    v->position.x = x;
    v->position.y = y;
    v->color = *col;
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
    sfVertexArray *va = {0};

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

static int start_menu(game_t *g, menu_t *m)
{
    if (init_sound(&g->sound, &g->settings) == EXIT_FAIL) {
        sfRenderWindow_destroy(g->window);
        return EXIT_FAIL;
    }
    sfMusic_play(g->sound.menu_music);
    if (init_menu(m, g) == EXIT_FAIL) {
        cleanup_game(g);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

int wolf(void)
{
    char **map = parsing_map(BASIC_MAP_PATH);
    game_t g = {0};
    menu_t m = {0};

    if (!map || game_init(&g) == EXIT_FAIL)
        return EXIT_FAIL;
    if (start_menu(&g, &m) == EXIT_FAIL)
        return EXIT_FAIL;
    run_menu(&m);
    g.selected_map = m.chosen_map;
    cleanup_menu(&m);
    if (m.action == MENU_PLAY)
        game_loop(map, &g);
    cleanup_game(&g);
    return EXIT_SUCCESS;
}
