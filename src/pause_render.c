/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pause_render.c
*/

#include "macros.h"
#include "proto.h"

void draw_overlay(sfRenderWindow *win, float ww, float wh)
{
    sfRectangleShape *r = sfRectangleShape_create();
    sfColor col = sfColor_fromRGB(10, 8, 8);

    if (!r)
        return;
    sfRectangleShape_setSize(r, (sfVector2f){ww, wh});
    sfRectangleShape_setFillColor(r, col);
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

static void draw_pause_title(pause_t *p, const char *str)
{
    sfText *txt = sfText_create();
    sfFloatRect lb = {0};
    sfVector2f pos = {0};

    if (!txt)
        return;
    sfText_setFont(txt, p->font);
    sfText_setString(txt, str);
    sfText_setCharacterSize(txt, TITLE_SZ);
    sfText_setFillColor(txt, COL_TITLE);
    lb = sfText_getLocalBounds(txt);
    pos.x = (p->ww - lb.width) / 2.0f - lb.left;
    pos.y = p->wh * 0.12f;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(p->window, txt, NULL);
    sfText_destroy(txt);
}

static void draw_pause_bar(pause_t *p, float val, float y)
{
    float cx = p->ww / 2.0f - 100.0f;
    sfFloatRect bg = {cx, y + 4.0f, 300.0f, 22.0f};
    sfFloatRect fill = {cx, y + 4.0f, val / VOL_MAX * 300.0f, 22.0f};
    sfColor dark = sfColor_fromRGB(20, 12, 6);
    sfColor orange = sfColor_fromRGB(210, 90, 15);

    draw_filled_rect(p->window, &bg, &dark);
    draw_filled_rect(p->window, &fill, &orange);
}

static void render_opt_row(pause_t *p, const char *lbl, float vol, float y)
{
    sfText *txt = sfText_create();
    sfVector2f pos = {p->ww / 2.0f - 250.0f, y};

    if (!txt)
        return;
    draw_pause_bar(p, vol, y);
    sfText_setFont(txt, p->font);
    sfText_setString(txt, lbl);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt, COL_LABEL);
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(p->window, txt, NULL);
    sfText_destroy(txt);
}

static void render_pause_opt(pause_t *p)
{
    const char *lbl0 = p->opt_sel == 0 ? "> Musique :" : "  Musique :";
    const char *lbl1 = p->opt_sel == 1 ? "> Sons :" : "  Sons :";

    draw_overlay(p->window, p->ww, p->wh);
    draw_pause_title(p, "OPTIONS");
    render_opt_row(p, lbl0, p->settings->music_vol, p->wh * 0.38f);
    render_opt_row(p, lbl1, p->settings->sfx_vol, p->wh * 0.55f);
    render_buttons(p->window, &p->opt_back, 1, -1);
}

static void render_pause_main(pause_t *p)
{
    draw_overlay(p->window, p->ww, p->wh);
    draw_pause_title(p, "PAUSE");
    render_buttons(p->window, p->btns, PAUSE_BTN_COUNT, p->sel);
}

void render_pause(pause_t *p)
{
    sfRenderWindow_clear(p->window, sfBlack);
    if (p->screen == PSCR_OPT)
        render_pause_opt(p);
    else
        render_pause_main(p);
    sfRenderWindow_display(p->window);
}
