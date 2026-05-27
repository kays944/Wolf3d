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
    sfVertexArray *va = NULL;

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

static int load_chosen_map(game_t *g, menu_t *m)
{
    char path[MAP_NAME_LEN + 32] = {0};

    free_map(&g->map);
    snprintf(path, sizeof(path), "%s/%s", MAP_DIR,
        m->map_names[m->chosen_map]);
    return parsing_map(&g->map, path);
}

static int run_cycle(game_t *g)
{
    menu_t m = {0};
    int action = 0;

    sfMusic_play(g->sound.menu_music);
    if (init_menu(&m, g) == EXIT_FAIL)
        return EXIT_FAIL;
    action = run_menu(&m);
    if (action == MENU_PLAY && load_chosen_map(g, &m) == EXIT_FAIL)
        action = MENU_QUIT;
    cleanup_menu(&m);
    if (action != MENU_PLAY)
        return EXIT_SUCCESS;
    return game_loop(g);
}

int wolf(void)
{
    game_t g = {0};
    int ret = PAUSE_MENU;

    if (game_init(&g) == EXIT_FAIL || init_sound(&g.sound, &g.settings)
        == EXIT_FAIL) {
        cleanup_game(&g);
        return EXIT_FAIL;
    }
    while (ret == PAUSE_MENU)
        ret = run_cycle(&g);
    cleanup_game(&g);
    return EXIT_SUCCESS;
}
