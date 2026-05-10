/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** settings_init.c
*/

#include "menu_proto.h"

static void create_settings_buttons(menu_t *m)
{
    float x = 0;
    float y = 0;
    sfVector2f p = {0};

    x = (m->ww - BTN_W) / 2.0f;
    y = m->wh - 120.0f;
    p = (sfVector2f){x, y};
    init_button(&m->set_btns[BTN_SET_BACK], &p, "RETOUR", m->font_med);
    m->set_btns[BTN_SET_BACK].id = BTN_SET_BACK;
}

void init_settings_menu(menu_t *m)
{
    m->settings_sel = 0;
    create_settings_buttons(m);
}

void cleanup_settings_menu(menu_t *m)
{
    for (int i = 0; i < SET_BTN_COUNT; i++)
        destroy_button(&m->set_btns[i]);
}
