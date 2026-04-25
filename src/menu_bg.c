/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_bg.c
*/

#include "menu_proto.h"

void render_background(menu_t *m)
{
    if (m->bg)
        sfRenderWindow_drawVertexArray(m->window, m->bg, NULL);
}

static void render_title_shadow(menu_t *m)
{
    sfColor shadow;
    sfVector2f pos;

    shadow = sfColor_fromRGBA(0, 0, 0, 180);
    pos = sfText_getPosition(m->title);
    sfText_setFillColor(m->title, shadow);
    sfText_setPosition(m->title, (sfVector2f){pos.x + 4, pos.y + 4});
    sfRenderWindow_drawText(m->window, m->title, NULL);
    sfText_setFillColor(m->title, sfColor_fromRGB(220, 50, 30));
    sfText_setPosition(m->title, pos);
}

void render_title(menu_t *m)
{
    draw_cfg_t cfg;

    if (!m->title)
        return;
    render_title_shadow(m);
    sfRenderWindow_drawText(m->window, m->title, NULL);
    cfg.x = 0;
    cfg.y = m->wh * 0.27f;
    cfg.sz = FONT_SMALL_SZ;
    cfg.col = sfColor_fromRGB(160, 160, 160);
    draw_text_centered(m, "WOLFENSTEIN 3D", &cfg);
}
