/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include "menu_proto.h"
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

static void handle_menu_state(game_t *g)
{
    menu_t menu = {0};

    if (init_menu(&menu, g) == -1) {
        g->running = sfFalse;
        return;
    }
    run_menu(&menu);
    g->selected_map = menu.chosen_map;
    if (menu.action == MENU_QUIT)
        g->running = sfFalse;
    if (menu.action == MENU_PLAY)
        g->state = STATE_GAME;
    cleanup_menu(&menu);
}

int wolf(void)
{
    char **map = parsing_map(BASIC_MAP_PATH);
    game_t g = {0};

    if (!map)
        return EXIT_FAIL;
    if (game_init(&g) == -1)
        return EXIT_FAIL;
    while (g.running && sfRenderWindow_isOpen(g.window)) {
        if (g.state == STATE_MENU)
            handle_menu_state(&g);
        if (g.state == STATE_GAME)
            return game_loop(map, &g);
    }
    game_cleanup(&g);
    return EXIT_SUCCESS;
}
