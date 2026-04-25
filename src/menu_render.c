/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_render.c
*/

#include "menu_proto.h"

void render_main_screen(menu_t *m)
{
    draw_cfg_t cfg;

    render_background(m);
    render_title(m);
    render_buttons(m->window, m->main_btns, MAIN_BTN_COUNT, m->selected);
    cfg = (draw_cfg_t){0, 650.0f, FONT_SMALL_SZ - 2,
        sfColor_fromRGBA(120, 120, 120, 200)};
    draw_text_centered(m, "Utilisez les fleches et Entree ou la souris", &cfg);
}

static void dispatch_render(menu_t *m)
{
    if (m->screen == SCR_MAIN)
        render_main_screen(m);
    if (m->screen == SCR_MAP_SELECT)
        render_map_select(m);
    if (m->screen == SCR_SETTINGS)
        render_settings(m);
}

void render_menu(menu_t *m)
{
    sfRenderWindow_clear(m->window, sfBlack);
    dispatch_render(m);
    sfRenderWindow_display(m->window);
}
