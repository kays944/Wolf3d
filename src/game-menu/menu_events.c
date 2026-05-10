/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_events.c
*/

#include "menu_proto.h"

static void update_hover(menu_t *m)
{
    for (int i = 0; i < MAIN_BTN_COUNT; i++)
        update_button(&m->main_btns[i], &m->mouse_pos);
}

static void on_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        navigate_menu(m, -1);
    if (e->key.code == sfKeyDown)
        navigate_menu(m, 1);
    if (e->key.code == sfKeyReturn)
        confirm_menu_selection(m);
    if (e->key.code == sfKeyEscape) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
}

void process_main_event(menu_t *m, sfEvent *e)
{
    sfVector2f click = {0};

    if (e->type == sfEvtClosed) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        update_hover(m);
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

static void dispatch_event(menu_t *m, sfEvent *e)
{
    if (m->screen == SCR_MAIN)
        process_main_event(m, e);
    if (m->screen == SCR_MAP_SELECT)
        handle_map_events(m, e);
    if (m->screen == SCR_SETTINGS)
        handle_settings_events(m, e);
}

void handle_menu_events(menu_t *m)
{
    sfEvent e = {0};

    while (sfRenderWindow_pollEvent(m->window, &e))
        dispatch_event(m, &e);
}
