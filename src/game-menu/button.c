/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button.c
*/

#include "proto.h"

static int btn_prepare(button_t *btn, const sfVector2f *pos)
{
    sfVector2f sz = {0};

    btn->pos = *pos;
    sz.x = BTN_W;
    sz.y = BTN_H;
    btn->size = sz;
    btn->hovered = sfFalse;
    btn->id = 0;
    btn->bg = sfRectangleShape_create();
    btn->label = sfText_create();
    if (!btn->bg || !btn->label) {
        if (btn->bg)
            sfRectangleShape_destroy(btn->bg);
        if (btn->label)
            sfText_destroy(btn->label);
        btn->bg = NULL;
        btn->label = NULL;
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static void btn_apply_style(button_t *btn, sfFont *font, const char *txt)
{
    sfRectangleShape_setSize(btn->bg, btn->size);
    sfRectangleShape_setPosition(btn->bg, btn->pos);
    sfRectangleShape_setOutlineThickness(btn->bg, 1.0f);
    sfText_setFont(btn->label, font);
    sfText_setString(btn->label, txt);
    sfText_setCharacterSize(btn->label, FONT_BTN_SZ);
    sfText_setFillColor(btn->label, sfWhite);
}

static void center_text(button_t *btn)
{
    sfFloatRect tb = {0};
    sfVector2f tpos = {0};
    float tx = 0;
    float ty = 0;

    tb = sfText_getGlobalBounds(btn->label);
    tx = btn->pos.x + (btn->size.x - tb.width) / 2.0f;
    ty = btn->pos.y + (btn->size.y - tb.height) / 2.0f - 4.0f;
    tpos.x = tx;
    tpos.y = ty;
    sfText_setPosition(btn->label, tpos);
}

int init_button(button_t *btn, const sfVector2f *pos,
    const char *txt, sfFont *font)
{
    if (btn_prepare(btn, pos) == EXIT_FAIL)
        return EXIT_FAIL;
    btn_apply_style(btn, font, txt);
    center_text(btn);
    return EXIT_SUCCESS;
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
    sfFloatRect b = {0};

    b.left = btn->pos.x;
    b.top = btn->pos.y;
    b.width = btn->size.x;
    b.height = btn->size.y;
    btn->hovered = sfFloatRect_contains(&b, mouse->x, mouse->y);
}

sfBool button_is_clicked(button_t *btn, const sfVector2f *mouse)
{
    sfFloatRect b = {0};

    b.left = btn->pos.x;
    b.top = btn->pos.y;
    b.width = btn->size.x;
    b.height = btn->size.y;
    return sfFloatRect_contains(&b, mouse->x, mouse->y);
}

static void render_button(sfRenderWindow *win, button_t *btn, sfBool sel)
{
    sfBool active = sfFalse;

    if (!btn->bg || !btn->label)
        return;
    active = btn->hovered || sel;
    if (active) {
        sfRectangleShape_setFillColor(btn->bg, COL_BTN_HOV);
        sfRectangleShape_setOutlineColor(btn->bg, COL_BTN_BORDER_HOV);
        sfText_setFillColor(btn->label, COL_SEL);
    } else {
        sfRectangleShape_setFillColor(btn->bg, COL_BTN);
        sfRectangleShape_setOutlineColor(btn->bg, COL_BTN_BORDER);
        sfText_setFillColor(btn->label, sfWhite);
    }
    sfRenderWindow_drawRectangleShape(win, btn->bg, NULL);
    sfRenderWindow_drawText(win, btn->label, NULL);
}

void render_buttons(sfRenderWindow *win, button_t *btns,
    int count, int selected)
{
    for (int i = 0; i < count; i++)
        render_button(win, &btns[i], (i == selected));
}


