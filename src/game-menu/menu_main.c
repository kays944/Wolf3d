/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_main.c
*/

#include "menu_proto.h"

static void update_delta(menu_t *m)
{
    sfTime elapsed = {0};

    elapsed = sfClock_restart(m->clock);
    m->dt = (double)sfTime_asSeconds(elapsed);
}

int run_menu(menu_t *m)
{
    while (sfRenderWindow_isOpen(m->window) && m->running) {
        update_delta(m);
        handle_menu_events(m);
        render_menu(m);
    }
    return m->action;
}
