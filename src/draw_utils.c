/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** draw_utils.c
*/

#include "menu_proto.h"

void draw_text_at(menu_t *m, const char *str, const draw_cfg_t *cfg)
{
    sfText *txt;

    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, str);
    sfText_setCharacterSize(txt, cfg->sz);
    sfText_setFillColor(txt, cfg->col);
    sfText_setPosition(txt, (sfVector2f){cfg->x, cfg->y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void draw_text_centered(menu_t *m, const char *str, const draw_cfg_t *cfg)
{
    sfText *txt;
    sfFloatRect bounds;
    draw_cfg_t c;

    txt = sfText_create();
    if (!txt)
        return;
    c = *cfg;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, str);
    sfText_setCharacterSize(txt, cfg->sz);
    sfText_setFillColor(txt, cfg->col);
    bounds = sfText_getLocalBounds(txt);
    c.x = (m->ww - bounds.width) / 2.0f - bounds.left;
    sfText_setPosition(txt, (sfVector2f){c.x, cfg->y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

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
