/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include "menu_proto.h"

static void handle_menu_state(game_t *g)
{
    menu_t menu;

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

static void handle_game_state(game_t *g)
{
    g->state = STATE_MENU;
}

int wolf(void)
{
    game_t g;

    if (game_init(&g) == -1)
        return EXIT_FAIL;
    while (g.running && sfRenderWindow_isOpen(g.window)) {
        if (g.state == STATE_MENU)
            handle_menu_state(&g);
        if (g.state == STATE_GAME)
            handle_game_state(&g);
    }
    game_cleanup(&g);
    return EXIT_SUCCESS;
}
