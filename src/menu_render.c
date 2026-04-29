/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_render.c
*/

#include "proto.h"

void draw_filled_rect(sfRenderWindow *win,
    const sfFloatRect *r, const sfColor *col)
{
    sfRectangleShape *rect;

    rect = sfRectangleShape_create();
    if (!rect)
        return;
    sfRectangleShape_setSize(rect, (sfVector2f){r->width, r->height});
    sfRectangleShape_setPosition(rect, (sfVector2f){r->left, r->top});
    sfRectangleShape_setFillColor(rect, *col);
    sfRenderWindow_drawRectangleShape(win, rect, NULL);
    sfRectangleShape_destroy(rect);
}

void draw_title(menu_t *m, const char *str, float y)
{
    sfText *txt;
    sfFloatRect lb;
    float x = 0;

    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_big);
    sfText_setString(txt, str);
    sfText_setCharacterSize(txt, TITLE_SZ);
    sfText_setFillColor(txt, COL_TITLE);
    lb = sfText_getLocalBounds(txt);
    x = (m->ww - lb.width) / 2.0f - lb.left;
    sfText_setPosition(txt, (sfVector2f){x, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void draw_hint(menu_t *m, const char *str, float y)
{
    sfText *txt;
    sfFloatRect lb;
    float x = 0;

    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, str);
    sfText_setCharacterSize(txt, FONT_SMALL_SZ);
    sfText_setFillColor(txt, COL_HINT);
    lb = sfText_getLocalBounds(txt);
    x = (m->ww - lb.width) / 2.0f - lb.left;
    sfText_setPosition(txt, (sfVector2f){x, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

static void render_background(menu_t *m)
{
    if (m->bg)
        sfRenderWindow_drawVertexArray(m->window, m->bg, NULL);
}

static void draw_title_shadow(menu_t *m)
{
    sfVector2f pos;

    pos = sfText_getPosition(m->title);
    sfText_setFillColor(m->title, sfColor_fromRGBA(0, 0, 0, 180));
    sfText_setPosition(m->title, (sfVector2f){pos.x + 4, pos.y + 4});
    sfRenderWindow_drawText(m->window, m->title, NULL);
    sfText_setFillColor(m->title, COL_TITLE);
    sfText_setPosition(m->title, pos);
}

static void render_title(menu_t *m)
{
    if (!m->title)
        return;
    draw_title_shadow(m);
    sfRenderWindow_drawText(m->window, m->title, NULL);
    draw_hint(m, "WOLFENSTEIN 3D", m->wh * 0.27f);
}

static void render_main_screen(menu_t *m)
{
    render_background(m);
    render_title(m);
    render_buttons(m->window, m->main_btns, MAIN_BTN_COUNT, m->selected);
    draw_hint(m, "Fleches / Entree / Souris", m->wh * 0.9f);
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
