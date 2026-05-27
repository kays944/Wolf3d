/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_events.c
*/

#include "proto.h"

static void confirm_menu_selection(menu_t *m)
{
    if (m->selected == BTN_QUIT) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
        return;
    }
    if (m->selected == BTN_PLAY) {
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

    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        for (int i = 0; i < MAIN_BTN_COUNT; i++)
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

static void reposition_btn(button_t *btn, const sfVector2f *pos)
{
    sfFloatRect tb = {0};
    sfVector2f tp = {0};

    btn->pos = *pos;
    if (btn->bg)
        sfRectangleShape_setPosition(btn->bg, btn->pos);
    if (!btn->label)
        return;
    tb = sfText_getGlobalBounds(btn->label);
    tp.x = pos->x + (btn->size.x - tb.width) / 2.0f;
    tp.y = pos->y + (btn->size.y - tb.height) / 2.0f - 4.0f;
    sfText_setPosition(btn->label, tp);
}

static void sync_window_size(menu_t *m)
{
    sfVector2u sz = sfRenderWindow_getSize(m->window);
    float x = 0;
    sfVector2f p = {0};

    m->ww = (float)sz.x;
    m->wh = (float)sz.y;
    x = (m->ww - BTN_W) / 2.0f;
    for (int i = 0; i < MAIN_BTN_COUNT; i++) {
        p.x = x;
        p.y = m->wh * 0.42f + i * (BTN_H + BTN_GAP);
        reposition_btn(&m->main_btns[i], &p);
    }
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
        sync_window_size(m);
        render_menu(m);
    }
    return m->action;
}
