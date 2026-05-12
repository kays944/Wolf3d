/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu.c
*/

#include "proto.h"

static void setup_title_position(menu_t *m)
{
    sfFloatRect lb = {0};
    sfVector2f p = {0};

    lb = sfText_getLocalBounds(m->title);
    p.x = (m->ww - lb.width) / 2.0f - lb.left;
    p.y = m->wh * 0.1f;
    sfText_setPosition(m->title, p);
}

static int setup_menu_visuals(menu_t *m)
{
    sfColor top = COL_BG_TOP;
    sfColor bot = COL_BG_BOT;

    m->bg = create_gradient_bg(&top, &bot, m->ww, m->wh);
    if (!m->bg)
        return -1;
    m->title = sfText_create();
    if (!m->title) {
        sfVertexArray_destroy(m->bg);
        m->bg = NULL;
        return EXIT_FAIL;
    }
    sfText_setFont(m->title, m->font_big);
    sfText_setString(m->title, "WOLF 3D");
    sfText_setCharacterSize(m->title, TITLE_SZ);
    sfText_setFillColor(m->title, COL_TITLE);
    sfText_setStyle(m->title, sfTextBold);
    setup_title_position(m);
    return EXIT_SUCCESS;
}

static int menu_clock_and_visuals(menu_t *m)
{
    m->clock = sfClock_create();
    if (!m->clock)
        return EXIT_FAIL;
    if (setup_menu_visuals(m) == EXIT_FAIL) {
        sfClock_destroy(m->clock);
        m->clock = NULL;
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static void create_buttons(menu_t *m)
{
    const char *labels[MAIN_BTN_COUNT];
    sfVector2f p = {0};
    float x = 0;

    labels[0] = "JOUER";
    labels[1] = "CHOISIR MAP";
    labels[2] = "PARAMETRES";
    labels[3] = "QUITTER";
    x = (m->ww - BTN_W) / 2.0f;
    for (int i = 0; i < MAIN_BTN_COUNT; i++) {
        p.x = x;
        p.y = m->wh * 0.42f + i * (BTN_H + BTN_GAP);
        init_button(&m->main_btns[i], &p, labels[i], m->font_med);
        m->main_btns[i].id = i;
    }
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
    if (menu_clock_and_visuals(m) == EXIT_FAIL)
        return EXIT_FAIL;
    create_buttons(m);
    return EXIT_SUCCESS;
}

void cleanup_menu(menu_t *m)
{
    save_settings(m->settings);
    for (int i = 0; i < MAIN_BTN_COUNT; i++)
        destroy_button(&m->main_btns[i]);
    cleanup_map_select(m);
    cleanup_settings_menu(m);
    if (m->title)
        sfText_destroy(m->title);
    if (m->bg)
        sfVertexArray_destroy(m->bg);
    if (m->clock)
        sfClock_destroy(m->clock);
}
