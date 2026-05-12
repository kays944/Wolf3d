/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_events.c
*/

#include "proto.h"

static void confirm_leave(menu_t *m)
{
    if (m->selected == BTN_PLAY)
        m->action = MENU_PLAY;
    else
        m->action = MENU_QUIT;
    m->running = sfFalse;
}

static void confirm_menu_selection(menu_t *m)
{
    if (m->selected == BTN_PLAY || m->selected == BTN_QUIT) {
        confirm_leave(m);
        return;
    }
    if (m->selected == BTN_MAP) {
        cleanup_map_select(m);
        init_map_select(m);
        m->screen = SCR_MAP_SELECT;
        return;
    }
    if (m->selected == BTN_SETTINGS) {
        cleanup_settings_menu(m);
        init_settings_menu(m);
        m->screen = SCR_SETTINGS;
    }
}

static void handle_mouse_click(menu_t *m, const sfVector2f *pos)
{
    for (int i = 0; i < MAIN_BTN_COUNT; i++) {
        if (button_is_clicked(&m->main_btns[i], pos)) {
            m->selected = i;
            confirm_menu_selection(m);
            return;
        }
    }
}

static void on_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp) {
        m->selected = m->selected - 1;
        if (m->selected < 0)
            m->selected = MAIN_BTN_COUNT - 1;
    }
    if (e->key.code == sfKeyDown) {
        m->selected = m->selected + 1;
        if (m->selected >= MAIN_BTN_COUNT)
            m->selected = 0;
    }
    if (e->key.code == sfKeyReturn)
        confirm_menu_selection(m);
    if (e->key.code == sfKeyEscape) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
}

static void process_main_event(menu_t *m, sfEvent *e)
{
    sfVector2f click = {0};
    int i = 0;

    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        for (i = 0; i < MAIN_BTN_COUNT; i++)
            update_button(&m->main_btns[i], &m->mouse_pos);
    }
    if (e->type == sfEvtKeyPressed)
        on_key(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        click.x = (float)e->mouseButton.x;
        click.y = (float)e->mouseButton.y;
        handle_mouse_click(m, &click);
    }
}

static void handle_menu_event(menu_t *m, sfEvent *e)
{
    int screen = m->screen;

    if (e->type == sfEvtClosed) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
    if (screen == SCR_MAIN)
        process_main_event(m, e);
    if (screen == SCR_MAP_SELECT)
        handle_map_events(m, e);
    if (screen == SCR_SETTINGS)
        handle_settings_events(m, e);
}

int run_menu(menu_t *m)
{
    sfTime elapsed = {0};
    sfEvent e = {0};

    while (sfRenderWindow_isOpen(m->window) && m->running) {
        elapsed = sfClock_restart(m->clock);
        m->dt = (double)sfTime_asSeconds(elapsed);
        while (sfRenderWindow_pollEvent(m->window, &e))
            handle_menu_event(m, &e);
        render_menu(m);
    }
    return m->action;
}
