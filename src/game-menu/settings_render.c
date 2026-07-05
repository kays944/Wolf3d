/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings_render.c
*/

#include "proto.h"

static void draw_bar(menu_t *m, float val, float y)
{
    sfFloatRect bg = {0};
    sfFloatRect fill = {0};
    sfColor col = {0};
    float fill_w = 0;

    bg.left = m->ww / 2.0f - 100.0f;
    bg.top = y + 4.0f;
    bg.width = 300.0f;
    bg.height = 22.0f;
    fill_w = val / VOL_MAX * 300.0f;
    fill.left = m->ww / 2.0f - 100.0f;
    fill.top = y + 4.0f;
    fill.width = fill_w;
    fill.height = 22.0f;
    col = sfColor_fromRGB(20, 12, 6);
    draw_filled_rect(m->window, &bg, &col);
    col = sfColor_fromRGB(210, 90, 15);
    draw_filled_rect(m->window, &fill, &col);
}

static void render_volume_bar(menu_t *m, const char *lbl, float val, float y)
{
    sfText *txt = sfText_create();
    sfVector2f pos = {0};

    if (!txt)
        return;
    draw_bar(m, val, y);
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, lbl);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt, COL_LABEL);
    pos.x = m->ww / 2.0f - 250.0f;
    pos.y = y;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

static sfText *draw_option_begin(menu_t *m, const char *lbl, float y)
{
    sfText *txt = sfText_create();
    sfVector2f pos = {0};

    if (!txt)
        return NULL;
    sfText_setFont(txt, m->font_med);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt, COL_LABEL);
    sfText_setString(txt, lbl);
    pos.x = m->ww / 2.0f - 250.0f;
    pos.y = y;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(m->window, txt, NULL);
    return txt;
}

static void draw_option_end(menu_t *m, sfText *txt,
    const char *val, int idx)
{
    float y = 200.0f + idx * 70.0f;
    sfVector2f pos = {0};

    if (m->settings_sel == idx)
        sfText_setFillColor(txt, COL_SEL);
    else
        sfText_setFillColor(txt, COL_LABEL);
    sfText_setString(txt, val);
    pos.x = m->ww / 2.0f - 100.0f;
    pos.y = y;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(m->window, txt, NULL);
}

static void draw_option(menu_t *m, const char *lbl, const char *val, int idx)
{
    float y = 200.0f + idx * 70.0f;
    sfText *txt = draw_option_begin(m, lbl, y);

    if (!txt)
        return;
    draw_option_end(m, txt, val, idx);
    sfText_destroy(txt);
}

static void render_settings_inner(menu_t *m)
{
    char buf[32];
    int w = 0;
    int h = 0;
    const char *fs = "OFF";

    render_volume_bar(m, "Musique :", m->settings->music_vol, 200.0f);
    render_volume_bar(m, "Sons :", m->settings->sfx_vol, 270.0f);
    get_resolution(m->settings->res_index, &w, &h);
    snprintf(buf, sizeof(buf), "< %d x %d >", w, h);
    draw_option(m, "Resolution :", buf, SET_RES);
    if (m->settings->fullscreen == sfTrue)
        fs = "ON";
    draw_option(m, "Plein ecran :", fs, SET_FULLSCR);
    if (m->settings->gamepad)
        draw_option(m, "Controles :", "< MANETTE >", SET_INPUT);
    else
        draw_option(m, "Controles :", "< CLAVIER >", SET_INPUT);
    snprintf(buf, sizeof(buf), "< %.1f >", m->settings->sensitivity);
    draw_option(m, "Sensibilite :", buf, SET_SENS);
}

void render_settings(menu_t *m)
{
    int back_sel = -1;

    render_menu_background(m);
    draw_title(m, "PARAMETRES", 60.0f);
    draw_hint(m, "Fleches haut/bas, gauche/droite", 130.0f);
    render_settings_inner(m);
    if (m->settings_sel == SET_BACK)
        back_sel = 0;
    render_buttons(m->window, m->set_btns, SET_BTN_COUNT, back_sel);
}
