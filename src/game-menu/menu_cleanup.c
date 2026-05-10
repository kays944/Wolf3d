/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_cleanup.c
*/

#include "menu_proto.h"

static void cleanup_main_buttons(menu_t *m)
{
    for (int i = 0; i < MAIN_BTN_COUNT; i++)
        destroy_button(&m->main_btns[i]);
}

void cleanup_menu(menu_t *m)
{
    save_settings(m->settings);
    cleanup_main_buttons(m);
    cleanup_map_select(m);
    cleanup_settings_menu(m);
    if (m->title)
        sfText_destroy(m->title);
    if (m->bg)
        sfVertexArray_destroy(m->bg);
    if (m->clock)
        sfClock_destroy(m->clock);
}
