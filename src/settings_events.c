/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings_events.c
*/

#include "menu_proto.h"

void adjust_volume(menu_t *m, int which, float delta)
{
    float *vol;

    vol = (which == SET_MUSIC) ? &m->settings->music_vol
        : &m->settings->sfx_vol;
    *vol += delta;
    if (*vol < VOL_MIN)
        *vol = VOL_MIN;
    if (*vol > VOL_MAX)
        *vol = VOL_MAX;
}

void cycle_resolution(menu_t *m, int dir)
{
    m->settings->res_index += dir;
    if (m->settings->res_index < 0)
        m->settings->res_index = NUM_RES - 1;
    if (m->settings->res_index >= NUM_RES)
        m->settings->res_index = 0;
    get_resolution(m->settings->res_index,
        &m->settings->win_w, &m->settings->win_h);
}

void toggle_fullscreen_setting(menu_t *m)
{
    m->settings->fullscreen = !m->settings->fullscreen;
}

static void adjust_setting(menu_t *m, int dir)
{
    if (m->settings_sel == SET_MUSIC || m->settings_sel == SET_SFX)
        adjust_volume(m, m->settings_sel, dir * VOL_STEP);
    if (m->settings_sel == SET_RES)
        cycle_resolution(m, dir);
    if (m->settings_sel == SET_FULLSCR)
        toggle_fullscreen_setting(m);
}

static void back_to_menu(menu_t *m)
{
    cleanup_settings_menu(m);
    m->screen = SCR_MAIN;
}

static void on_set_hover(menu_t *m, sfEvent *e)
{
    int i;

    m->mouse_pos.x = (float)e->mouseMove.x;
    m->mouse_pos.y = (float)e->mouseMove.y;
    for (i = 0; i < SET_BTN_COUNT; i++)
        update_button(&m->set_btns[i], &m->mouse_pos);
}

static void on_set_click(menu_t *m, sfEvent *e)
{
    sfVector2f pos;

    pos.x = (float)e->mouseButton.x;
    pos.y = (float)e->mouseButton.y;
    if (button_is_clicked(&m->set_btns[BTN_SET_BACK], &pos))
        back_to_menu(m);
}

static void on_set_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp) {
        m->settings_sel--;
        if (m->settings_sel < 0)
            m->settings_sel = SET_ITEM_COUNT - 1;
    }
    if (e->key.code == sfKeyDown) {
        m->settings_sel++;
        if (m->settings_sel >= SET_ITEM_COUNT)
            m->settings_sel = 0;
    }
    if (e->key.code == sfKeyLeft)
        adjust_setting(m, -1);
    if (e->key.code == sfKeyRight)
        adjust_setting(m, 1);
    if (e->key.code == sfKeyReturn && m->settings_sel == SET_BACK)
        back_to_menu(m);
    if (e->key.code == sfKeyEscape)
        back_to_menu(m);
}

void handle_settings_events(menu_t *m, sfEvent *e)
{
    if (e->type == sfEvtClosed) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
    if (e->type == sfEvtMouseMoved)
        on_set_hover(m, e);
    if (e->type == sfEvtKeyPressed)
        on_set_key(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft)
        on_set_click(m, e);
}
