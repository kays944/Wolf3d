/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button.c
*/

#include "button_proto.h"

static void center_button_text(button_t *btn)
{
    sfFloatRect tb;
    float tx;
    float ty;

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
    center_button_text(btn);
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
    sfFloatRect bounds;

    bounds = (sfFloatRect){btn->pos.x, btn->pos.y,
        btn->size.x, btn->size.y};
    btn->hovered = sfFloatRect_contains(&bounds, mouse->x, mouse->y);
}

sfBool button_is_clicked(button_t *btn, const sfVector2f *mouse)
{
    sfFloatRect bounds;

    bounds = (sfFloatRect){btn->pos.x, btn->pos.y,
        btn->size.x, btn->size.y};
    return sfFloatRect_contains(&bounds, mouse->x, mouse->y);
}
