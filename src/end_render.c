/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** end_render.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

sfVertexArray *make_end_bg(player_t *p, int mode)
{
    sfColor top = END_BG_TOP;
    sfColor bot = (mode == END_MODE_LOSE) ? END_BG_LOSE : END_BG_WIN;

    return create_gradient_bg(&top, &bot, (float)p->ww, (float)p->wh);
}

static const float GLOW_OFF[4][2] = {{-4, 0}, {4, 0}, {0, -4}, {0, 4}};

static void draw_glow(sfRenderWindow *win, sfText *t, float base)
{
    sfVector2f p0 = sfText_getPosition(t);
    sfVector2f q = {0};

    sfText_setScale(t, (sfVector2f){base, base});
    sfText_setFillColor(t, sfColor_fromRGBA(255, 105, 20, 60));
    for (int i = 0; i < 4; i++) {
        q.x = p0.x + GLOW_OFF[i][0];
        q.y = p0.y + GLOW_OFF[i][1];
        sfText_setPosition(t, q);
        sfRenderWindow_drawText(win, t, NULL);
    }
    sfText_setPosition(t, p0);
}

static void draw_end_word(sfRenderWindow *win, sfText *t, float el)
{
    float base = 1.0f + 0.07f * sinf(el * 3.2f);
    int g = 170 + (int)(55.0f * sinf(el * 2.4f));

    draw_glow(win, t, base);
    sfText_setScale(t, (sfVector2f){base, base});
    sfText_setFillColor(t, sfColor_fromRGB(255, g, 45));
    sfRenderWindow_drawText(win, t, NULL);
}

static void draw_bar(sfRenderWindow *win, player_t *p, float el)
{
    float f = (el < 0.45f) ? el / 0.45f : 1.0f;
    float w = END_BAR_W * f;
    sfRectangleShape *r = sfRectangleShape_create();
    sfColor col = COL_TITLE;

    if (!r)
        return;
    sfRectangleShape_setSize(r, (sfVector2f){w, END_BAR_H});
    sfRectangleShape_setPosition(r, (sfVector2f){(p->ww - w) / 2.0f,
            p->wh * END_BAR_Y});
    sfRectangleShape_setFillColor(r, col);
    sfRenderWindow_drawRectangleShape(win, r, NULL);
    sfRectangleShape_destroy(r);
}

static void draw_stats(sfRenderWindow *win, player_t *p)
{
    sfText *t = sfText_create();
    char buf[48] = {0};
    sfFloatRect b = {0};

    if (!t)
        return;
    snprintf(buf, sizeof(buf), "KILLS %d    SCORE %d", p->kills, p->score);
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, buf);
    sfText_setCharacterSize(t, 30);
    sfText_setFillColor(t, COL_LABEL);
    b = sfText_getLocalBounds(t);
    sfText_setPosition(t, (sfVector2f){(p->ww - b.width) / 2.0f - b.left,
            p->wh * 0.37f});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}

void draw_end_scene(sfRenderWindow *win, player_t *p, sfText *t, float el)
{
    draw_bar(win, p, el);
    if (t)
        draw_end_word(win, t, el);
    draw_stats(win, p);
}
