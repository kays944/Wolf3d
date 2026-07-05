/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** end_menu.c
*/

#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "proto.h"

static const char *END_LABELS[3][3] = {
    {"RECOMMENCER", "MENU", NULL},
    {"NIVEAU SUIVANT", "RECOMMENCER", "MENU"},
    {"RECOMMENCER", "MENU", NULL},
};

static const int END_CODES[3][3] = {
    {END_RESTART, PAUSE_MENU, 0},
    {END_NEXT, END_RESTART, PAUSE_MENU},
    {END_RESTART, PAUSE_MENU, 0},
};

int next_level_path(map_t *m, char *buf, int size)
{
    int n = 0;
    char *base = m->path ? strrchr(m->path, '/') : NULL;

    if (!base || sscanf(base, "/level%d.wolf", &n) != 1)
        return EXIT_FAIL;
    snprintf(buf, size, "%s/level%d%s", MAP_DIR, n + 1, MAP_EXT);
    return EXIT_SUCCESS;
}

static int end_btn_count(int mode)
{
    return mode == END_MODE_WIN_NEXT ? 3 : 2;
}

static void init_end_buttons(button_t *btns, player_t *p, int mode)
{
    sfVector2f pos = {0};
    int n = end_btn_count(mode);

    for (int i = 0; i < n; i++) {
        pos.x = (p->ww - BTN_W) / 2.0f;
        pos.y = p->wh * END_BTN_Y + i * (BTN_H + BTN_GAP);
        init_button(&btns[i], &pos, END_LABELS[mode][i], p->hud_font);
        btns[i].id = i;
    }
}

static sfText *make_end_title(player_t *p, const char *msg)
{
    sfText *t = sfText_create();
    sfFloatRect b = {0};

    if (!t)
        return NULL;
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, msg);
    sfText_setCharacterSize(t, END_FONT_SZ);
    sfText_setFillColor(t, COL_TITLE);
    b = sfText_getLocalBounds(t);
    sfText_setOrigin(t, (sfVector2f){b.left + b.width / 2.0f,
            b.top + b.height / 2.0f});
    sfText_setPosition(t, (sfVector2f){p->ww / 2.0f, p->wh * END_TITLE_Y});
    return t;
}

static void end_nav(int *sel, int n, int dir)
{
    *sel += dir;
    if (*sel < 0)
        *sel = n - 1;
    if (*sel >= n)
        *sel = 0;
}

static int end_mouse(button_t *btns, int n, sfEvent *e, int *sel)
{
    sfVector2f pos = {(float)e->mouseButton.x, (float)e->mouseButton.y};

    for (int i = 0; i < n; i++)
        if (button_is_clicked(&btns[i], &pos)) {
            *sel = i;
            return 1;
        }
    return 0;
}

static int end_event(sfEvent *e, int *sel, int n, sfRenderWindow *win)
{
    int act = pad_menu_action(e);

    if (e->type == sfEvtClosed) {
        sfRenderWindow_close(win);
        return 1;
    }
    if (e->type == sfEvtKeyPressed && e->key.code == sfKeyUp)
        end_nav(sel, n, -1);
    if (e->type == sfEvtKeyPressed && e->key.code == sfKeyDown)
        end_nav(sel, n, 1);
    if (e->type == sfEvtKeyPressed && e->key.code == sfKeyReturn)
        return 1;
    if (act == PM_UP)
        end_nav(sel, n, -1);
    if (act == PM_DOWN)
        end_nav(sel, n, 1);
    if (act == PM_OK)
        return 1;
    return 0;
}

static int end_poll(sfRenderWindow *win, button_t *btns, int n, int *sel)
{
    sfEvent e;
    sfVector2f mp = {0};

    while (sfRenderWindow_pollEvent(win, &e)) {
        if (e.type == sfEvtMouseMoved) {
            mp.x = (float)e.mouseMove.x;
            mp.y = (float)e.mouseMove.y;
            for (int i = 0; i < n; i++) {
                update_button(&btns[i], &mp);
                if (btns[i].hovered)
                    *sel = i;
            }
        }
        if (e.type == sfEvtMouseButtonPressed
            && e.mouseButton.button == sfMouseLeft
            && end_mouse(btns, n, &e, sel))
            return 1;
        if (end_event(&e, sel, n, win))
            return 1;
    }
    return 0;
}

static void end_frame(sfRenderWindow *win, player_t *p, end_ctx_t *c)
{
    float el = c->clock ? sfTime_asSeconds(sfClock_getElapsedTime(c->clock))
        : 0.0f;

    sfRenderWindow_clear(win, sfBlack);
    if (c->bg)
        sfRenderWindow_drawVertexArray(win, c->bg, NULL);
    draw_end_scene(win, p, c->title, el);
    render_buttons(win, c->btns, c->n, c->sel);
    sfRenderWindow_display(win);
}

static void end_cleanup(end_ctx_t *c)
{
    if (c->title)
        sfText_destroy(c->title);
    if (c->bg)
        sfVertexArray_destroy(c->bg);
    if (c->clock)
        sfClock_destroy(c->clock);
    for (int i = 0; i < c->n; i++)
        destroy_button(&c->btns[i]);
}

int run_end_menu(sfRenderWindow *win, player_t *p,
    const char *title, int mode)
{
    button_t btns[3] = {0};
    end_ctx_t c = {btns, make_end_title(p, title), make_end_bg(p, mode),
        sfClock_create(), end_btn_count(mode), 0};
    int done = 0;

    init_end_buttons(btns, p, mode);
    while (sfRenderWindow_isOpen(win) && !done) {
        done = end_poll(win, btns, c.n, &c.sel);
        end_frame(win, p, &c);
    }
    end_cleanup(&c);
    if (!sfRenderWindow_isOpen(win))
        return PAUSE_RESUME;
    return END_CODES[mode][c.sel];
}
