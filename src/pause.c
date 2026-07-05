/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pause.c
*/

#include "macros.h"
#include "proto.h"

static void setup_btns(pause_t *p)
{
    const char *labels[PAUSE_BTN_COUNT];
    sfVector2f pos = {0};
    float x = (p->ww - BTN_W) / 2.0f;
    int i = 0;

    labels[PBTN_OPT] = "OPTIONS";
    labels[PBTN_SAVE] = "SAUVEGARDER";
    labels[PBTN_BACK] = "REVENIR AU MENU";
    labels[PBTN_QUIT_ID] = "QUITTER";
    for (i = 0; i < PAUSE_BTN_COUNT; i++) {
        pos.x = x;
        pos.y = p->wh * 0.38f + i * (BTN_H + BTN_GAP);
        init_button(&p->btns[i], &pos, labels[i], p->font);
        p->btns[i].id = i;
    }
}

static int init_pause(pause_t *p, game_t *g, map_t *map, player_t *player)
{
    sfVector2u sz = sfRenderWindow_getSize(g->window);
    sfVector2f bpos = {0};

    memset(p, 0, sizeof(pause_t));
    p->window = g->window;
    p->font = g->font_big;
    p->sound = &g->sound;
    p->settings = &g->settings;
    p->map = map;
    p->player = player;
    p->ww = (float)sz.x;
    p->wh = (float)sz.y;
    p->running = sfTrue;
    p->action = PAUSE_RESUME;
    p->screen = PSCR_MAIN;
    setup_btns(p);
    bpos.x = (p->ww - BTN_W) / 2.0f;
    bpos.y = p->wh * 0.75f;
    init_button(&p->opt_back, &bpos, "< RETOUR", p->font);
    return EXIT_SUCCESS;
}

static void cleanup_pause(pause_t *p)
{
    int i = 0;

    save_settings(p->settings);
    for (i = 0; i < PAUSE_BTN_COUNT; i++)
        destroy_button(&p->btns[i]);
    destroy_button(&p->opt_back);
}

static void adjust_vol(pause_t *p, int dir)
{
    float *vol = NULL;

    if (p->opt_sel == 0)
        vol = &p->settings->music_vol;
    if (p->opt_sel == 1)
        vol = &p->settings->sfx_vol;
    if (!vol)
        return;
    *vol += dir * VOL_STEP;
    if (*vol < VOL_MIN)
        *vol = VOL_MIN;
    if (*vol > VOL_MAX)
        *vol = VOL_MAX;
    update_sound_vol(p->sound, p->settings);
}

static void handle_opt_key(pause_t *p, sfEvent *e)
{
    if (e->key.code == sfKeyEscape) {
        p->screen = PSCR_MAIN;
        return;
    }
    if (e->key.code == sfKeyUp || e->key.code == sfKeyDown)
        p->opt_sel = p->opt_sel == 0 ? 1 : 0;
    if (e->key.code == sfKeyLeft)
        adjust_vol(p, -1);
    if (e->key.code == sfKeyRight)
        adjust_vol(p, 1);
}

static void handle_opt_click(pause_t *p, sfEvent *e)
{
    sfVector2f pos = {0};

    if (e->type == sfEvtMouseMoved) {
        p->mouse.x = (float)e->mouseMove.x;
        p->mouse.y = (float)e->mouseMove.y;
        update_button(&p->opt_back, &p->mouse);
    }
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        pos.x = (float)e->mouseButton.x;
        pos.y = (float)e->mouseButton.y;
        if (button_is_clicked(&p->opt_back, &pos))
            p->screen = PSCR_MAIN;
    }
}

static void pause_activate(pause_t *p, int id)
{
    if (id == PBTN_OPT) {
        p->screen = PSCR_OPT;
        return;
    }
    if (id == PBTN_SAVE) {
        save_game(p->map, p->player);
        return;
    }
    if (id == PBTN_BACK) {
        p->action = PAUSE_MENU;
        p->running = sfFalse;
        return;
    }
    if (id == PBTN_QUIT_ID) {
        p->action = PAUSE_QUIT;
        p->running = sfFalse;
    }
}

static void handle_click(pause_t *p, sfVector2f *pos)
{
    for (int i = 0; i < PAUSE_BTN_COUNT; i++)
        if (button_is_clicked(&p->btns[i], pos)) {
            pause_activate(p, i);
            return;
        }
}

static void nav_pause(pause_t *p, int dir)
{
    p->sel = p->sel + dir;
    if (p->sel < 0)
        p->sel = PAUSE_BTN_COUNT - 1;
    if (p->sel >= PAUSE_BTN_COUNT)
        p->sel = 0;
}

static void on_pause_key(pause_t *p, sfEvent *e)
{
    if (e->key.code == sfKeyEscape) {
        p->action = PAUSE_RESUME;
        p->running = sfFalse;
    }
    if (e->key.code == sfKeyUp)
        nav_pause(p, -1);
    if (e->key.code == sfKeyDown)
        nav_pause(p, 1);
    if (e->key.code == sfKeyReturn)
        pause_activate(p, p->sel);
}

static void on_pause_pad(pause_t *p, sfEvent *e)
{
    int act = pad_menu_action(e);

    if (act == PM_UP)
        nav_pause(p, -1);
    if (act == PM_DOWN)
        nav_pause(p, 1);
    if (act == PM_OK)
        pause_activate(p, p->sel);
    if (act == PM_BACK) {
        p->action = PAUSE_RESUME;
        p->running = sfFalse;
    }
}

static void handle_pause_main(pause_t *p, sfEvent *e)
{
    sfVector2f pos = {0};

    if (e->type == sfEvtMouseMoved) {
        p->mouse.x = (float)e->mouseMove.x;
        p->mouse.y = (float)e->mouseMove.y;
        for (int i = 0; i < PAUSE_BTN_COUNT; i++)
            update_button(&p->btns[i], &p->mouse);
    }
    if (e->type == sfEvtKeyPressed)
        on_pause_key(p, e);
    on_pause_pad(p, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        pos.x = (float)e->mouseButton.x;
        pos.y = (float)e->mouseButton.y;
        handle_click(p, &pos);
    }
}

static void on_opt_pad(pause_t *p, sfEvent *e)
{
    int act = pad_menu_action(e);

    if (act == PM_UP || act == PM_DOWN)
        p->opt_sel = p->opt_sel == 0 ? 1 : 0;
    if (act == PM_LEFT)
        adjust_vol(p, -1);
    if (act == PM_RIGHT)
        adjust_vol(p, 1);
    if (act == PM_BACK || act == PM_OK)
        p->screen = PSCR_MAIN;
}

static void handle_pause_event(pause_t *p, sfEvent *e)
{
    if (e->type == sfEvtClosed) {
        p->action = PAUSE_QUIT;
        p->running = sfFalse;
        return;
    }
    if (e->type == sfEvtJoystickButtonPressed
        && e->joystickButton.button == PAD_BTN_PAUSE
        && p->screen == PSCR_MAIN) {
        p->action = PAUSE_RESUME;
        p->running = sfFalse;
        return;
    }
    if (p->screen == PSCR_OPT && e->type == sfEvtKeyPressed)
        handle_opt_key(p, e);
    if (p->screen == PSCR_OPT) {
        on_opt_pad(p, e);
        handle_opt_click(p, e);
    }
    if (p->screen == PSCR_MAIN)
        handle_pause_main(p, e);
}

int run_pause(game_t *g, map_t *map, player_t *player)
{
    pause_t p = {0};
    sfEvent e = {0};

    if (init_pause(&p, g, map, player) == EXIT_FAIL)
        return PAUSE_QUIT;
    while (sfRenderWindow_isOpen(p.window) && p.running) {
        while (sfRenderWindow_pollEvent(p.window, &e))
            handle_pause_event(&p, &e);
        render_pause(&p);
    }
    cleanup_pause(&p);
    return p.action;
}
