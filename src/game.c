/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static sfRenderWindow *creation_window(void)
{
    sfVideoMode mode = {WIN_WIDTH, WIN_HEIGHT, FREQUENCY};
    sfRenderWindow *window = {0};

    window = sfRenderWindow_create(mode, "fen1", sfResize | sfClose, NULL);
    if (window)
        sfRenderWindow_setFramerateLimit(window, 60);
    return window;
}

static int find_in_row(char *row, int *r)
{
    int i = 0;

    while (i < MAP_WIDTH) {
        if (row[i] == 'o') {
            *r = i;
            return EXIT_SUCCESS;
        }
        i++;
    }
    return EXIT_FAIL;
}

static int find_start(char **map, int *col, int *row)
{
    int c = 0;

    while (c < MAP_HEIGHT) {
        if (find_in_row(map[c], row) == EXIT_SUCCESS) {
            *col = c;
            return EXIT_SUCCESS;
        }
        c++;
    }
    return EXIT_FAIL;
}

static int init_player(char **map, player_t *player)
{
    int col = 0;
    int row = 0;

    if (find_start(map, &col, &row) == EXIT_FAIL)
        return EXIT_FAIL;
    player->x = row * TILE_SIZE + TILE_SIZE / 2;
    player->y = col * TILE_SIZE + TILE_SIZE / 2;
    player->angle = 0;
    return EXIT_SUCCESS;
}

void cleanup_game(game_t *g)
{
    destroy_sound(&g->sound);
    if (g->font_big)
        sfFont_destroy(g->font_big);
    if (g->window)
        sfRenderWindow_destroy(g->window);
}

int game_loop(char **map, game_t *g)
{
    sfRenderWindow *window = creation_window();
    player_t player = {0};

    if (!window || init_player(map, &player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_weapon(&player) == EXIT_FAIL) {
        close_all(window);
        return EXIT_FAIL;
    }
    sfMusic_stop(g->sound.menu_music);
    sfMusic_play(g->sound.game_music);
    while (sfRenderWindow_isOpen(window)) {
        if (event(window, &player, map, &g->sound) == EVENT_CLOSE)
            break;
        draw(window, &player, map);
    }
    sfMusic_stop(g->sound.game_music);
    destroy_weapon(&player);
    close_all(window);
    return EXIT_SUCCESS;
}
