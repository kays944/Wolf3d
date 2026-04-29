/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings.c
*/

#include "proto.h"

int init_settings_menu(menu_t *m)
{
    sfVector2f p;

    m->settings_sel = 0;
    p = (sfVector2f){(m->ww - BTN_W) / 2.0f, m->wh - 120.0f};
    init_button(&m->set_btns[BTN_SET_BACK], &p, "RETOUR", m->font_med);
    m->set_btns[BTN_SET_BACK].id = BTN_SET_BACK;
    return 0;
}

void cleanup_settings_menu(menu_t *m)
{
    int i = 0;

    for (i = 0; i < SET_BTN_COUNT; i++)
        destroy_button(&m->set_btns[i]);
}

static void cycle_resolution(menu_t *m, int dir)
{
    m->settings->res_index = (m->settings->res_index + dir + NUM_RES) % NUM_RES;
    get_resolution(m->settings->res_index,
        &m->settings->win_w, &m->settings->win_h);
}

static void on_set_nav(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        m->settings_sel = (m->settings_sel - 1 + SET_ITEM_COUNT)
            % SET_ITEM_COUNT;
    if (e->key.code == sfKeyDown)
        m->settings_sel = (m->settings_sel + 1) % SET_ITEM_COUNT;
    if (e->key.code == sfKeyReturn && m->settings_sel == SET_BACK) {
        cleanup_settings_menu(m);
        m->screen = SCR_MAIN;
    }
    if (e->key.code == sfKeyEscape) {
        cleanup_settings_menu(m);
        m->screen = SCR_MAIN;
    }
}

static void on_set_adjust(menu_t *m, sfEvent *e)
{
    float *vol = NULL;
    int dir = 0;

    if (e->key.code != sfKeyLeft && e->key.code != sfKeyRight)
        return;
    dir = (e->key.code == sfKeyLeft) ? -1 : 1;
    vol = NULL;
    if (m->settings_sel == SET_MUSIC)
        vol = &m->settings->music_vol;
    if (m->settings_sel == SET_SFX)
        vol = &m->settings->sfx_vol;
    if (vol) {
        *vol += dir * VOL_STEP;
        *vol = (float)fmax(VOL_MIN, fmin(VOL_MAX, *vol));
    }
    if (m->settings_sel == SET_RES)
        cycle_resolution(m, dir);
    if (m->settings_sel == SET_FULLSCR)
        m->settings->fullscreen = !m->settings->fullscreen;
}

void handle_settings_events(menu_t *m, sfEvent *e)
{
    sfVector2f pos;

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

static void draw_bar(menu_t *m, float val, float y)
{
    sfFloatRect bg;
    sfFloatRect fill;
    sfColor col;

    bg = (sfFloatRect){m->ww / 2.0f - 100.0f, y + 4, 300.0f, 22};
    fill = (sfFloatRect){m->ww / 2.0f - 100.0f, y + 4,
        val / VOL_MAX * 300.0f, 22};
    col = sfColor_fromRGB(50, 50, 50);
    draw_filled_rect(m->window, &bg, &col);
    col = sfColor_fromRGB(180, 40, 20);
    draw_filled_rect(m->window, &fill, &col);
}

static void render_volume_bar(menu_t *m, const char *lbl, float val, float y)
{
    sfText *txt;

    draw_bar(m, val, y);
    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, lbl);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt,
        COL_LABEL);
    sfText_setPosition(txt, (sfVector2f){m->ww / 2.0f - 250.0f, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

static void draw_option(menu_t *m, const char *lbl, const char *val, int idx)
{
    sfText *txt = sfText_create();
    float y = 200.0f + idx * 70.0f;

    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt, COL_LABEL);
    sfText_setString(txt, lbl);
    sfText_setPosition(txt, (sfVector2f){m->ww / 2.0f - 250.0f, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_setFillColor(txt, m->settings_sel == idx ? COL_SEL : COL_LABEL);
    sfText_setString(txt, val);
    sfText_setPosition(txt, (sfVector2f){m->ww / 2.0f - 100.0f, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void render_settings(menu_t *m)
{
    char buf[32];
    int w = 0;
    int h = 0;

    if (m->bg)
        sfRenderWindow_drawVertexArray(m->window, m->bg, NULL);
    draw_title(m, "PARAMETRES", 60.0f);
    draw_hint(m, "Fleches haut/bas, gauche/droite", 130.0f);
    render_volume_bar(m, "Musique :", m->settings->music_vol, 200.0f);
    render_volume_bar(m, "Sons :", m->settings->sfx_vol, 270.0f);
    get_resolution(m->settings->res_index, &w, &h);
    snprintf(buf, sizeof(buf), "< %d x %d >", w, h);
    draw_option(m, "Resolution :", buf, SET_RES);
    draw_option(m, "Plein ecran :",
        m->settings->fullscreen ? "ON" : "OFF", SET_FULLSCR);
    render_buttons(m->window, m->set_btns, SET_BTN_COUNT,
        m->settings_sel == SET_BACK ? 0 : -1);
}
