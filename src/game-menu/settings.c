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
    init_button(&m->set_btns[BTN_SET_BACK], &p, "RETOUR", m->font_med);
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

static void on_set_nav(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp) {
        m->settings_sel = m->settings_sel - 1;
        if (m->settings_sel < 0)
            m->settings_sel = SET_ITEM_COUNT - 1;
    }
    if (e->key.code == sfKeyDown) {
        m->settings_sel = m->settings_sel + 1;
        if (m->settings_sel >= SET_ITEM_COUNT)
            m->settings_sel = 0;
    }
    if ((e->key.code == sfKeyReturn && m->settings_sel == SET_BACK)
        || e->key.code == sfKeyEscape) {
        cleanup_settings_menu(m);
        m->screen = SCR_MAIN;
    }
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

void handle_settings_events(menu_t *m, sfEvent *e)
{
    sfVector2f pos = {0};

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
    fclose(f);
}
