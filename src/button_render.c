/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button_render.c
*/

#include "button_proto.h"

static sfColor get_btn_color(sfBool active)
{
    if (active)
        return sfColor_fromRGBA(COL_BTN_HOV_R, COL_BTN_HOV_G,
            COL_BTN_HOV_B, COL_BTN_HOV_A);
    return sfColor_fromRGBA(COL_BTN_R, COL_BTN_G, COL_BTN_B, COL_BTN_A);
}

static sfColor get_outline_color(sfBool active)
{
    if (active)
        return sfColor_fromRGB(200, 60, 40);
    return sfColor_fromRGB(80, 80, 80);
}

static sfColor get_text_color(sfBool active)
{
    if (active)
        return sfColor_fromRGB(255, 220, 0);
    return sfWhite;
}

void render_button(sfRenderWindow *win, button_t *btn, sfBool sel)
{
    sfBool active;

    if (!btn->bg || !btn->label)
        return;
    active = btn->hovered || sel;
    sfRectangleShape_setFillColor(btn->bg, get_btn_color(active));
    sfRectangleShape_setOutlineColor(btn->bg, get_outline_color(active));
    sfText_setFillColor(btn->label, get_text_color(active));
    sfRenderWindow_drawRectangleShape(win, btn->bg, NULL);
    sfRenderWindow_drawText(win, btn->label, NULL);
}

void render_buttons(sfRenderWindow *win, button_t *btns,
    int count, int selected)
{
    int i;

    for (i = 0; i < count; i++)
        render_button(win, &btns[i], (i == selected));
}
