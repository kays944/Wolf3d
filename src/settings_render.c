/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings_render.c
*/

#include "menu_proto.h"

static sfColor item_color(menu_t *m, int idx)
{
    if (m->settings_sel == idx)
        return sfColor_fromRGB(255, 220, 0);
    return sfColor_fromRGB(200, 200, 200);
}

void render_volume_bar(menu_t *m, const char *lbl, float val, float y)
{
    float bx;
    float bw;
    sfFloatRect bg;
    sfFloatRect fill;
    sfColor bg_col;
    sfColor fill_col;
    draw_cfg_t cfg;

    bx = m->ww / 2.0f - 100.0f;
    bw = 300.0f;
    cfg = (draw_cfg_t){m->ww / 2.0f - 250.0f, y, FONT_LABEL_SZ,
        sfColor_fromRGB(200, 200, 200)};
    draw_text_at(m, lbl, &cfg);
    bg = (sfFloatRect){bx, y + 4, bw, 22};
    fill = (sfFloatRect){bx, y + 4, val / VOL_MAX * bw, 22};
    bg_col = sfColor_fromRGB(50, 50, 50);
    fill_col = sfColor_fromRGB(180, 40, 20);
    draw_filled_rect(m->window, &bg, &bg_col);
    draw_filled_rect(m->window, &fill, &fill_col);
}

void render_res_selector(menu_t *m, float y)
{
    int w;
    int h;
    char buf[32];
    draw_cfg_t cfg;

    get_resolution(m->settings->res_index, &w, &h);
    snprintf(buf, sizeof(buf), "< %d x %d >", w, h);
    cfg = (draw_cfg_t){m->ww / 2.0f - 250.0f, y, FONT_LABEL_SZ,
        sfColor_fromRGB(200, 200, 200)};
    draw_text_at(m, "Resolution :", &cfg);
    cfg = (draw_cfg_t){m->ww / 2.0f - 100.0f, y, FONT_LABEL_SZ,
        item_color(m, SET_RES)};
    draw_text_at(m, buf, &cfg);
}

void render_fullscreen_toggle(menu_t *m, float y)
{
    draw_cfg_t cfg;

    cfg = (draw_cfg_t){m->ww / 2.0f - 250.0f, y, FONT_LABEL_SZ,
        sfColor_fromRGB(200, 200, 200)};
    draw_text_at(m, "Plein ecran :", &cfg);
    cfg = (draw_cfg_t){m->ww / 2.0f - 100.0f, y, FONT_LABEL_SZ,
        item_color(m, SET_FULLSCR)};
    draw_text_at(m, m->settings->fullscreen ? "ON" : "OFF", &cfg);
}

void render_settings(menu_t *m)
{
    draw_cfg_t cfg;

    render_background(m);
    cfg = (draw_cfg_t){0, 60.0f, 50, sfColor_fromRGB(220, 50, 30)};
    draw_text_centered(m, "PARAMETRES", &cfg);
    cfg = (draw_cfg_t){0, 130.0f, FONT_SMALL_SZ,
        sfColor_fromRGB(150, 150, 150)};
    draw_text_centered(m,
        "Fleches haut/bas pour naviguer, gauche/droite pour modifier", &cfg);
    render_volume_bar(m, "Musique :", m->settings->music_vol, 200.0f);
    render_volume_bar(m, "Sons :", m->settings->sfx_vol, 270.0f);
    render_res_selector(m, 340.0f);
    render_fullscreen_toggle(m, 410.0f);
    render_buttons(m->window, m->set_btns, SET_BTN_COUNT,
        m->settings_sel == SET_BACK ? 0 : -1);
}
