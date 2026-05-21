/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static int find_in_row(char *row, int *r)
{
    int i = 0;

    while (row[i]) {
        if (row[i] == 'o') {
            *r = i;
            return EXIT_SUCCESS;
        }
        i++;
    }
    return EXIT_FAIL;
}

static int find_start(map_t *m, int *col, int *row)
{
    int c = 0;

    while (c < m->size_y) {
        if (find_in_row(m->map[c], row) == EXIT_SUCCESS) {
            *col = c;
            return EXIT_SUCCESS;
        }
        c++;
    }
    return EXIT_FAIL;
}

static int init_player(map_t *m, player_t *player)
{
    int col = 0;
    int row = 0;

    if (find_start(m, &col, &row) == EXIT_FAIL)
        return EXIT_FAIL;
    player->x = row * TILE_SIZE + TILE_SIZE / 2;
    player->y = col * TILE_SIZE + TILE_SIZE / 2;
    player->angle = 0;
    return EXIT_SUCCESS;
}

void cleanup_game(game_t *g)
{
    free_map(&g->map);
    destroy_sound(&g->sound);
    if (g->font_big)
        sfFont_destroy(g->font_big);
    if (g->window)
        sfRenderWindow_destroy(g->window);
}

static int init_player_tools(player_t *player)
{
    if (init_weapon(player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_flashlight(player) == EXIT_FAIL) {
        destroy_weapon(player);
        return EXIT_FAIL;
    }
    if (init_health_bar(player) == EXIT_FAIL) {
        destroy_flashlight(player);
        destroy_weapon(player);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static int run_game(sfRenderWindow *w, player_t *p, game_t *g)
{
    int ev = 0;
    int ret = PAUSE_RESUME;

    sfMusic_stop(g->sound.menu_music);
    sfMusic_play(g->sound.game_music);
    while (sfRenderWindow_isOpen(w)) {
        ev = event(w, p, &g->map, &g->sound);
        if (ev == EVENT_CLOSE)
            break;
        if (ev == EVENT_PAUSE)
            ret = run_pause(g, &g->map, p);
        if (ret != PAUSE_RESUME)
            break;
        draw(w, p, &g->map);
    }
    sfMusic_stop(g->sound.game_music);
    return ret;
}

int game_loop(game_t *g)
{
    sfVector2u sz = sfRenderWindow_getSize(g->window);
    player_t player = {0};
    int ret = 0;

    player.ww = (int)sz.x;
    player.wh = (int)sz.y;
    if (init_player(&g->map, &player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_player_tools(&player) == EXIT_FAIL)
        return EXIT_FAIL;
    ret = run_game(g->window, &player, g);
    destroy_health_bar(&player);
    destroy_flashlight(&player);
    destroy_weapon(&player);
    return ret;
}
