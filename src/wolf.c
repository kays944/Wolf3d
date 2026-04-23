/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include "macros.h"
#include "wolf.h"
#include <stdio.h>
#include <math.h>

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

static int game_loop(char **map)
{
    sfRenderWindow *window = creation_window();
    player_t player = {0};

    if (!window || init_player(map, &player) == EXIT_FAIL)
        return EXIT_FAIL;
    while (sfRenderWindow_isOpen(window)) {
        if (event(window) == EVENT_CLOSE)
            break;
        draw(window, &player, map);
    }
    close_all(window);
    return EXIT_SUCCESS;
}

int wolf(void)
{
    char **map = parsing_map(BASIC_MAP_PATH);

    if (!map)
        return EXIT_FAIL;
    return game_loop(map);
}
