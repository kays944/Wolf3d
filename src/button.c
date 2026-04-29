/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button.c
*/

#include "proto.h"

static void center_text(button_t *btn)
{
    sfFloatRect tb;
    float tx = 0;
    float ty = 0;

    tb = sfText_getGlobalBounds(btn->label);
    tx = btn->pos.x + (btn->size.x - tb.width) / 2.0f;
    ty = btn->pos.y + (btn->size.y - tb.height) / 2.0f - 4.0f;
    sfText_setPosition(btn->label, (sfVector2f){tx, ty});
}

int init_button(button_t *btn, const sfVector2f *pos,
    const char *txt, sfFont *font)
{
    btn->pos = *pos;
    btn->size = (sfVector2f){BTN_W, BTN_H};
    btn->hovered = sfFalse;
    btn->id = 0;
    btn->bg = sfRectangleShape_create();
    btn->label = sfText_create();
    if (!btn->bg || !btn->label)
        return -1;
    sfRectangleShape_setSize(btn->bg, btn->size);
    sfRectangleShape_setPosition(btn->bg, btn->pos);
    sfRectangleShape_setOutlineThickness(btn->bg, 2.0f);
    sfText_setFont(btn->label, font);
    sfText_setString(btn->label, txt);
    sfText_setCharacterSize(btn->label, FONT_BTN_SZ);
    sfText_setFillColor(btn->label, sfWhite);
    center_text(btn);
    return 0;
}

void destroy_button(button_t *btn)
{
    if (btn->bg)
        sfRectangleShape_destroy(btn->bg);
    if (btn->label)
        sfText_destroy(btn->label);
    btn->bg = NULL;
    btn->label = NULL;
}

void update_button(button_t *btn, const sfVector2f *mouse)
{
    sfFloatRect b;

    b = (sfFloatRect){btn->pos.x, btn->pos.y, btn->size.x, btn->size.y};
    btn->hovered = sfFloatRect_contains(&b, mouse->x, mouse->y);
}

sfBool button_is_clicked(button_t *btn, const sfVector2f *mouse)
{
    sfFloatRect b;

    b = (sfFloatRect){btn->pos.x, btn->pos.y, btn->size.x, btn->size.y};
    return sfFloatRect_contains(&b, mouse->x, mouse->y);
}

static sfColor btn_fill(sfBool active)
{
    if (active)
        return COL_BTN_HOV;
    return COL_BTN;
}

static sfColor btn_outline(sfBool active)
{
    if (active)
        return sfColor_fromRGB(200, 60, 40);
    return sfColor_fromRGB(80, 80, 80);
}

static void render_button(sfRenderWindow *win, button_t *btn, sfBool sel)
{
    sfBool active;

    if (!btn->bg || !btn->label)
        return;
    active = btn->hovered || sel;
    sfRectangleShape_setFillColor(btn->bg, btn_fill(active));
    sfRectangleShape_setOutlineColor(btn->bg, btn_outline(active));
    sfText_setFillColor(btn->label,
        active ? sfColor_fromRGB(255, 220, 0) : sfWhite);
    sfRenderWindow_drawRectangleShape(win, btn->bg, NULL);
    sfRenderWindow_drawText(win, btn->label, NULL);
}

void render_buttons(sfRenderWindow *win, button_t *btns,
    int count, int selected)
{
    int i = 0;

    for (i = 0; i < count; i++)
        render_button(win, &btns[i], (i == selected));
}
