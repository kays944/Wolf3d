/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_render.c
*/

#include "proto.h"

static void apply_ui_view(menu_t *m)
{
    sfFloatRect r = {0, 0, m->ww, m->wh};
    sfView *v = sfView_createFromRect(r);

    if (!v)
        return;
    sfRenderWindow_setView(m->window, v);
    sfView_destroy(v);
}

static void draw_bg_sprite(menu_t *m)
{
    sfVector2u tsz = sfTexture_getSize(m->bg_tex);
    sfFloatRect img = {0, 0, (float)tsz.x, (float)tsz.y};
    sfView *view = sfView_createFromRect(img);

    if (!view)
        return;
    sfRenderWindow_setView(m->window, view);
    sfRenderWindow_drawSprite(m->window, m->bg_spr, NULL);
    sfView_destroy(view);
    apply_ui_view(m);
}

void render_menu_background(menu_t *m)
{
    if (m->bg_tex && m->bg_spr) {
        draw_bg_sprite(m);
        return;
    }
    if (m->bg)
        sfRenderWindow_drawVertexArray(m->window, m->bg, NULL);
}

void draw_filled_rect(sfRenderWindow *win,
    const sfFloatRect *r, const sfColor *col)
{
    sfRectangleShape *rect = {0};
    sfVector2f size = {0};
    sfVector2f position = {0};

    rect = sfRectangleShape_create();
    if (!rect)
        return;
    size.x = r->width;
    size.y = r->height;
    position.x = r->left;
    position.y = r->top;
    sfRectangleShape_setSize(rect, size);
    sfRectangleShape_setPosition(rect, position);
    sfRectangleShape_setFillColor(rect, *col);
    sfRenderWindow_drawRectangleShape(win, rect, NULL);
    sfRectangleShape_destroy(rect);
}

void draw_title(menu_t *m, const char *str, float y)
{
    sfText *txt = {0};
    sfFloatRect lb = {0};
    sfVector2f pos = {0};
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
    pos.x = x;
    pos.y = y;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void draw_hint(menu_t *m, const char *str, float y)
{
    sfText *txt = {0};
    sfFloatRect lb = {0};
    sfVector2f pos = {0};
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
    pos.x = x;
    pos.y = y;
    sfText_setPosition(txt, pos);
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

static void draw_title_shadow(menu_t *m)
{
    sfVector2f pos = sfText_getPosition(m->title);
    sfVector2f shadow = {pos.x + 5.0f, pos.y + 5.0f};

    sfText_setFillColor(m->title, sfColor_fromRGBA(60, 5, 0, 210));
    sfText_setPosition(m->title, shadow);
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
}

static void render_main_screen(menu_t *m)
{
    sfFloatRect r = {0};
    sfColor col = {0};

    render_menu_background(m);
    setup_title_position(m);
    render_title(m);
    r.width = BTN_W + 60.0f;
    r.height = MAIN_BTN_COUNT * (BTN_H + BTN_GAP) + 30.0f;
    r.left = (m->ww - r.width) / 2.0f;
    r.top = m->wh * 0.40f;
    col = sfColor_fromRGBA(5, 2, 1, 130);
    draw_filled_rect(m->window, &r, &col);
    render_buttons(m->window, m->main_btns, MAIN_BTN_COUNT, m->selected);
    draw_hint(m, "Fleches / Entree / Souris", m->wh * 0.9f);
}

void render_menu(menu_t *m)
{
    apply_ui_view(m);
    sfRenderWindow_clear(m->window, sfBlack);
    if (m->screen == SCR_MAIN)
        render_main_screen(m);
    if (m->screen == SCR_MAP_SELECT)
        render_map_select(m);
    if (m->screen == SCR_SETTINGS)
        render_settings(m);
    sfRenderWindow_display(m->window);
}
