/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_init.c
*/

#include "menu_proto.h"

static void create_main_buttons(menu_t *m)
{
    float x = 0;
    float y = 0;
    sfVector2f p = {0};
    const char *labels[MAIN_BTN_COUNT] = {"JOUER", "CHOISIR MAP",
        "PARAMETRES", "QUITTER"};

    x = (m->ww - BTN_W) / 2.0f;
    y = m->wh * 0.42f;
    for (int i = 0; i < MAIN_BTN_COUNT; i++) {
        p = (sfVector2f){x, y + i * (BTN_H + BTN_GAP)};
        init_button(&m->main_btns[i], &p, labels[i], m->font_med);
        m->main_btns[i].id = i;
    }
}

static int init_background(menu_t *m)
{
    sfColor top = {0};
    sfColor bot = {0};

    top = sfColor_fromRGB(COL_BG_TOP_R, COL_BG_TOP_G, COL_BG_TOP_B);
    bot = sfColor_fromRGB(COL_BG_BOT_R, COL_BG_BOT_G, COL_BG_BOT_B);
    m->bg = create_gradient_bg(&top, &bot, m->ww, m->wh);
    return m->bg ? 0 : -1;
}

static int init_title(menu_t *m)
{
    sfFloatRect bounds = {0};
    float x = 0;

    m->title = sfText_create();
    if (!m->title)
        return EXIT_FAIL;
    sfText_setFont(m->title, m->font_big);
    sfText_setString(m->title, "WOLF 3D");
    sfText_setCharacterSize(m->title, FONT_TITLE_SZ);
    sfText_setFillColor(m->title, sfColor_fromRGB(220, 50, 30));
    sfText_setStyle(m->title, sfTextBold);
    bounds = sfText_getLocalBounds(m->title);
    x = (m->ww - bounds.width) / 2.0f - bounds.left;
    sfText_setPosition(m->title, (sfVector2f){x, m->wh * 0.1f});
    return EXIT_SUCCESS;
}

int init_menu(menu_t *m, game_t *g)
{
    sfVector2u sz = {0};

    memset(m, 0, sizeof(menu_t));
    m->window = g->window;
    m->font_big = g->font_big;
    m->font_med = g->font_med;
    m->settings = &g->settings;
    sz = sfRenderWindow_getSize(m->window);
    m->ww = (float)sz.x;
    m->wh = (float)sz.y;
    m->screen = SCR_MAIN;
    m->running = sfTrue;
    m->action = MENU_QUIT;
    m->clock = sfClock_create();
    if (!m->clock)
        return EXIT_FAIL;
    if (init_background(m) == EXIT_FAIL || init_title(m) == EXIT_FAIL)
        return EXIT_FAIL;
    create_main_buttons(m);
    return EXIT_SUCCESS;
}
