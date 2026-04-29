/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu.c
*/

#include "proto.h"

static void create_buttons(menu_t *m)
{
    const char *lbl[MAIN_BTN_COUNT] = {"JOUER", "CHOISIR MAP",
        "PARAMETRES", "QUITTER"};
    sfVector2f p;
    float x = 0;
    int i = 0;

    x = (m->ww - BTN_W) / 2.0f;
    for (i = 0; i < MAIN_BTN_COUNT; i++) {
        p = (sfVector2f){x, m->wh * 0.42f + i * (BTN_H + BTN_GAP)};
        init_button(&m->main_btns[i], &p, lbl[i], m->font_med);
        m->main_btns[i].id = i;
    }
}

static int setup_menu_visuals(menu_t *m)
{
    sfColor top = COL_BG_TOP;
    sfColor bot = COL_BG_BOT;

    m->bg = create_gradient_bg(&top, &bot, m->ww, m->wh);
    if (!m->bg)
        return -1;
    m->title = sfText_create();
    if (!m->title)
        return -1;
    sfText_setFont(m->title, m->font_big);
    sfText_setString(m->title, "WOLF 3D");
    sfText_setCharacterSize(m->title, TITLE_SZ);
    sfText_setFillColor(m->title, COL_TITLE);
    sfText_setStyle(m->title, sfTextBold);
    sfText_setPosition(m->title, (sfVector2f){
            (m->ww - sfText_getLocalBounds(m->title).width) / 2.0f
            - sfText_getLocalBounds(m->title).left, m->wh * 0.1f});
    return 0;
}

int init_menu(menu_t *m, game_t *g)
{
    sfVector2u sz;

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
    if (!m->clock || setup_menu_visuals(m) == -1)
        return -1;
    create_buttons(m);
    return 0;
}

void cleanup_menu(menu_t *m)
{
    int i = 0;

    save_settings(m->settings);
    for (i = 0; i < MAIN_BTN_COUNT; i++)
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

static void confirm_menu_selection(menu_t *m)
{
    if (m->selected == BTN_PLAY || m->selected == BTN_QUIT) {
        m->action = (m->selected == BTN_PLAY) ? MENU_PLAY : MENU_QUIT;
        m->running = sfFalse;
        return;
    }
    if (m->selected == BTN_MAP) {
        cleanup_map_select(m);
        init_map_select(m);
        m->screen = SCR_MAP_SELECT;
    }
    if (m->selected == BTN_SETTINGS) {
        cleanup_settings_menu(m);
        init_settings_menu(m);
        m->screen = SCR_SETTINGS;
    }
}

static void handle_mouse_click(menu_t *m, const sfVector2f *pos)
{
    int i = 0;

    for (i = 0; i < MAIN_BTN_COUNT; i++) {
        if (button_is_clicked(&m->main_btns[i], pos)) {
            m->selected = i;
            confirm_menu_selection(m);
            return;
        }
    }
}

static void on_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        m->selected = (m->selected - 1 + MAIN_BTN_COUNT) % MAIN_BTN_COUNT;
    if (e->key.code == sfKeyDown)
        m->selected = (m->selected + 1) % MAIN_BTN_COUNT;
    if (e->key.code == sfKeyReturn)
        confirm_menu_selection(m);
    if (e->key.code == sfKeyEscape) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
}

static void process_main_event(menu_t *m, sfEvent *e)
{
    sfVector2f click;
    int i = 0;

    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        for (i = 0; i < MAIN_BTN_COUNT; i++)
            update_button(&m->main_btns[i], &m->mouse_pos);
    }
    if (e->type == sfEvtKeyPressed)
        on_key(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        click.x = (float)e->mouseButton.x;
        click.y = (float)e->mouseButton.y;
        handle_mouse_click(m, &click);
    }
}

static void dispatch_event(menu_t *m, sfEvent *e)
{
    if (e->type == sfEvtClosed) {
        m->action = MENU_QUIT;
        m->running = sfFalse;
    }
    if (m->screen == SCR_MAIN)
        process_main_event(m, e);
    if (m->screen == SCR_MAP_SELECT)
        handle_map_events(m, e);
    if (m->screen == SCR_SETTINGS)
        handle_settings_events(m, e);
}

int run_menu(menu_t *m)
{
    sfTime elapsed;
    sfEvent e;

    while (sfRenderWindow_isOpen(m->window) && m->running) {
        elapsed = sfClock_restart(m->clock);
        m->dt = (double)sfTime_asSeconds(elapsed);
        while (sfRenderWindow_pollEvent(m->window, &e))
            dispatch_event(m, &e);
        render_menu(m);
    }
    return m->action;
}
