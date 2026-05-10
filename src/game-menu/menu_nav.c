/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_nav.c
*/

#include "menu_proto.h"

void navigate_menu(menu_t *m, int dir)
{
    m->selected += dir;
    if (m->selected < 0)
        m->selected = MAIN_BTN_COUNT - 1;
    if (m->selected >= MAIN_BTN_COUNT)
        m->selected = 0;
}

static void open_map_select(menu_t *m)
{
    cleanup_map_select(m);
    init_map_select(m);
    m->screen = SCR_MAP_SELECT;
}

static void open_settings(menu_t *m)
{
    cleanup_settings_menu(m);
    init_settings_menu(m);
    m->screen = SCR_SETTINGS;
}

void confirm_menu_selection(menu_t *m)
{
    if (m->selected == BTN_PLAY) {
        m->action = MENU_PLAY;
        m->running = sfFalse;
    }
    if (m->selected == BTN_MAP)
        open_map_select(m);
    if (m->selected == BTN_SETTINGS)
        open_settings(m);
    if (m->selected == BTN_QUIT) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
}

void handle_mouse_click(menu_t *m, const sfVector2f *mouse)
{
    for (int i = 0; i < MAIN_BTN_COUNT; i++) {
        if (button_is_clicked(&m->main_btns[i], mouse)) {
            m->selected = i;
            confirm_menu_selection(m);
            return;
        }
    }
}
