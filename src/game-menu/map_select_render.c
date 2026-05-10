/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_select_render.c
*/

#include "menu_proto.h"

static void render_map_header(menu_t *m)
{
    draw_cfg_t cfg;

    render_background(m);
    cfg = (draw_cfg_t){0, 60.0f, 50, sfColor_fromRGB(220, 50, 30)};
    draw_text_centered(m, "CHOISIR UNE MAP", &cfg);
    cfg = (draw_cfg_t){0, 130.0f, FONT_SMALL_SZ,
        sfColor_fromRGB(150, 150, 150)};
    draw_text_centered(m, "Fleches pour naviguer, Entree pour selectionner",
        &cfg);
}

static void render_map_item(menu_t *m, int i, float y)
{
    sfColor bg_col;
    sfColor txt_col;
    sfFloatRect bg;
    draw_cfg_t cfg;
    float lx;

    lx = (m->ww - 600.0f) / 2.0f;
    bg_col = (i == m->map_selected)
        ? sfColor_fromRGBA(110, 25, 15, 220)
        : sfColor_fromRGBA(30, 30, 30, 180);
    txt_col = (i == m->map_selected)
        ? sfColor_fromRGB(255, 220, 0) : sfWhite;
    bg = (sfFloatRect){lx - 10, y - 5, 620, 40};
    draw_filled_rect(m->window, &bg, &bg_col);
    cfg = (draw_cfg_t){lx, y, FONT_LABEL_SZ, txt_col};
    draw_text_at(m, m->map_names[i], &cfg);
}

void render_map_list(menu_t *m)
{
    draw_cfg_t cfg;
    float start_y;
    int i;

    start_y = 200.0f;
    if (m->map_count == 0) {
        cfg = (draw_cfg_t){0, 350.0f, FONT_LABEL_SZ,
            sfColor_fromRGB(150, 150, 150)};
        draw_text_centered(m, "Aucune map disponible", &cfg);
        return;
    }
    for (i = 0; i < m->map_count; i++)
        render_map_item(m, i, start_y + i * 50.0f);
}

void render_map_select(menu_t *m)
{
    render_map_header(m);
    render_map_list(m);
    render_buttons(m->window, m->map_btns, MAP_BTN_COUNT, -1);
}
