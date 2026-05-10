/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_select_events.c
*/

#include "menu_proto.h"

void navigate_map_list(menu_t *m, int dir)
{
    if (m->map_count == 0)
        return;
    m->map_selected += dir;
    if (m->map_selected < 0)
        m->map_selected = m->map_count - 1;
    if (m->map_selected >= m->map_count)
        m->map_selected = 0;
}

void confirm_map_selection(menu_t *m)
{
    if (m->map_count == 0)
        return;
    m->chosen_map = m->map_selected;
    m->action = MENU_PLAY;
    m->running = sfFalse;
}

static void back_to_menu(menu_t *m)
{
    cleanup_map_select(m);
    m->screen = SCR_MAIN;
}

static void on_map_hover(menu_t *m, sfEvent *e)
{
    m->mouse_pos.x = (float)e->mouseMove.x;
    m->mouse_pos.y = (float)e->mouseMove.y;
    for (int i = 0; i < MAP_BTN_COUNT; i++)
        update_button(&m->map_btns[i], &m->mouse_pos);
}

static void on_map_click(menu_t *m, sfEvent *e)
{
    sfVector2f pos = {0};

    pos.x = (float)e->mouseButton.x;
    pos.y = (float)e->mouseButton.y;
    if (button_is_clicked(&m->map_btns[BTN_MAP_PLAY], &pos))
        confirm_map_selection(m);
    if (button_is_clicked(&m->map_btns[BTN_MAP_BACK], &pos))
        back_to_menu(m);
}

static void on_map_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        navigate_map_list(m, -1);
    if (e->key.code == sfKeyDown)
        navigate_map_list(m, 1);
    if (e->key.code == sfKeyReturn)
        confirm_map_selection(m);
    if (e->key.code == sfKeyEscape)
        back_to_menu(m);
}

void handle_map_events(menu_t *m, sfEvent *e)
{
    if (e->type == sfEvtClosed) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
    if (e->type == sfEvtMouseMoved)
        on_map_hover(m, e);
    if (e->type == sfEvtKeyPressed)
        on_map_key(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft)
        on_map_click(m, e);
}
