/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings.c
*/

#include "proto.h"

int init_settings_menu(menu_t *m)
{
    sfVector2f p = {0};

    m->settings_sel = 0;
    p.x = (m->ww - BTN_W) / 2.0f;
    p.y = m->wh - 120.0f;
    init_button(&m->set_btns[BTN_SET_BACK], &p, "BACK", m->font_med);
    m->set_btns[BTN_SET_BACK].id = BTN_SET_BACK;
    return EXIT_SUCCESS;
}

void cleanup_settings_menu(menu_t *m)
{
    for (int i = 0; i < SET_BTN_COUNT; i++)
        destroy_button(&m->set_btns[i]);
}

static void cycle_resolution(menu_t *m, int dir)
{
    int idx = 0;

    idx = m->settings->res_index + dir;
    if (idx < 0)
        idx = NUM_RES - 1;
    if (idx >= NUM_RES)
        idx = 0;
    m->settings->res_index = idx;
    get_resolution(idx, &m->settings->win_w, &m->settings->win_h);
}

static void nav_settings(menu_t *m, int dir)
{
    m->settings_sel = m->settings_sel + dir;
    if (m->settings_sel < 0)
        m->settings_sel = SET_ITEM_COUNT - 1;
    if (m->settings_sel >= SET_ITEM_COUNT)
        m->settings_sel = 0;
}

static void leave_settings(menu_t *m)
{
    cleanup_settings_menu(m);
    m->screen = SCR_MAIN;
}

static void on_set_nav(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        nav_settings(m, -1);
    if (e->key.code == sfKeyDown)
        nav_settings(m, 1);
    if ((e->key.code == sfKeyReturn && m->settings_sel == SET_BACK)
        || e->key.code == sfKeyEscape)
        leave_settings(m);
}

static void apply_vol_step(float *vol, int dir)
{
    *vol = *vol + dir * VOL_STEP;
    if (*vol < VOL_MIN)
        *vol = VOL_MIN;
    if (*vol > VOL_MAX)
        *vol = VOL_MAX;
}

static void toggle_fullscr(settings_t *s)
{
    if (s->fullscreen == sfTrue)
        s->fullscreen = sfFalse;
    else
        s->fullscreen = sfTrue;
}

static void cycle_diff(settings_t *s, int dir)
{
    s->difficulty += dir;
    if (s->difficulty < 0)
        s->difficulty = DIFF_COUNT - 1;
    if (s->difficulty >= DIFF_COUNT)
        s->difficulty = 0;
}

static void adjust_sens(settings_t *s, int dir)
{
    s->sensitivity += dir * SENS_STEP;
    if (s->sensitivity < SENS_MIN)
        s->sensitivity = SENS_MIN;
    if (s->sensitivity > SENS_MAX)
        s->sensitivity = SENS_MAX;
}

static void adjust_vol_or_setting(menu_t *m, int dir)
{
    float *vol = NULL;

    if (m->settings_sel == SET_MUSIC)
        vol = &m->settings->music_vol;
    if (m->settings_sel == SET_SFX)
        vol = &m->settings->sfx_vol;
    if (vol != NULL) {
        apply_vol_step(vol, dir);
        update_sound_vol(m->sound, m->settings);
        return;
    }
    if (m->settings_sel == SET_RES)
        cycle_resolution(m, dir);
    if (m->settings_sel == SET_FULLSCR)
        toggle_fullscr(m->settings);
    if (m->settings_sel == SET_INPUT)
        m->settings->gamepad = m->settings->gamepad ? 0 : 1;
    if (m->settings_sel == SET_SENS)
        adjust_sens(m->settings, dir);
    if (m->settings_sel == SET_DIFF)
        cycle_diff(m->settings, dir);
}

static void on_set_adjust(menu_t *m, sfEvent *e)
{
    int dir = 0;

    if (e->key.code != sfKeyLeft && e->key.code != sfKeyRight)
        return;
    if (e->key.code == sfKeyLeft)
        dir = -1;
    else
        dir = 1;
    adjust_vol_or_setting(m, dir);
}

static void on_set_pad(menu_t *m, sfEvent *e)
{
    int act = pad_menu_action(e);

    if (act == PM_UP)
        nav_settings(m, -1);
    if (act == PM_DOWN)
        nav_settings(m, 1);
    if (act == PM_LEFT)
        adjust_vol_or_setting(m, -1);
    if (act == PM_RIGHT)
        adjust_vol_or_setting(m, 1);
    if (act == PM_BACK || (act == PM_OK && m->settings_sel == SET_BACK)) {
        leave_settings(m);
        return;
    }
    if (act == PM_OK)
        adjust_vol_or_setting(m, 1);
}

void handle_settings_events(menu_t *m, sfEvent *e)
{
    sfVector2f pos = {0};

    on_set_pad(m, e);
    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        update_button(&m->set_btns[0], &m->mouse_pos);
    }
    if (e->type == sfEvtKeyPressed) {
        on_set_nav(m, e);
        on_set_adjust(m, e);
    }
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        pos.x = (float)e->mouseButton.x;
        pos.y = (float)e->mouseButton.y;
        if (button_is_clicked(&m->set_btns[BTN_SET_BACK], &pos)) {
            cleanup_settings_menu(m);
            m->screen = SCR_MAIN;
        }
    }
}

void save_settings(settings_t *s)
{
    FILE *f = NULL;

    f = fopen(CFG_PATH, "w");
    if (!f)
        return;
    fprintf(f, "music_vol %f\n", s->music_vol);
    fprintf(f, "sfx_vol %f\n", s->sfx_vol);
    fprintf(f, "res_index %d\n", s->res_index);
    fprintf(f, "fullscreen %d\n", s->fullscreen);
    fprintf(f, "gamepad %d\n", s->gamepad);
    fprintf(f, "sensitivity %f\n", s->sensitivity);
    fprintf(f, "difficulty %d\n", s->difficulty);
    fclose(f);
}
