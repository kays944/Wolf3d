/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_select.c
*/

#include "proto.h"

static int keep_map(const char *name, size_t elen)
{
    size_t nlen = strlen(name);

    if (nlen <= elen || strcmp(name + nlen - elen, MAP_EXT) != 0)
        return 0;
    if (strcmp(name, "map_save.wolf") == 0)
        return 0;
    return 1;
}

static void sort_maps(menu_t *m)
{
    char tmp[MAP_NAME_LEN] = {0};

    for (int i = 0; i < m->map_count - 1; i++)
        for (int j = 0; j < m->map_count - 1 - i; j++)
            if (strcmp(m->map_names[j], m->map_names[j + 1]) > 0) {
                snprintf(tmp, MAP_NAME_LEN, "%s", m->map_names[j]);
                snprintf(m->map_names[j], MAP_NAME_LEN, "%s",
                    m->map_names[j + 1]);
                snprintf(m->map_names[j + 1], MAP_NAME_LEN, "%s", tmp);
            }
}

static void scan_maps(menu_t *m)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    size_t elen = strlen(MAP_EXT);

    m->map_count = 0;
    dir = opendir(MAP_DIR);
    if (!dir)
        return;
    for (entry = readdir(dir);
        entry && m->map_count < MAX_MAPS;
        entry = readdir(dir)) {
        if (keep_map(entry->d_name, elen)) {
            snprintf(m->map_names[m->map_count], MAP_NAME_LEN, "%s",
                entry->d_name);
            m->map_count++;
        }
    }
    closedir(dir);
    sort_maps(m);
}

int init_map_select(menu_t *m)
{
    sfVector2f p = {0};
    float cx = 0;

    m->map_selected = 0;
    scan_maps(m);
    cx = (m->ww / 2.0f) - BTN_W - BTN_GAP / 2.0f;
    p.x = cx;
    p.y = m->wh - 120.0f;
    init_button(&m->map_btns[BTN_MAP_PLAY], &p, "PLAY", m->font_med);
    p.x = cx + BTN_W + BTN_GAP;
    p.y = m->wh - 120.0f;
    init_button(&m->map_btns[BTN_MAP_BACK], &p, "BACK", m->font_med);
    return EXIT_SUCCESS;
}

void cleanup_map_select(menu_t *m)
{
    for (int i = 0; i < MAP_BTN_COUNT; i++)
        destroy_button(&m->map_btns[i]);
}

static void confirm_map_selection(menu_t *m)
{
    if (m->map_count == 0)
        return;
    m->chosen_map = m->map_selected;
    m->action = MENU_PLAY;
    m->running = sfFalse;
}

static void nav_map(menu_t *m, int dir)
{
    if (m->map_count <= 0)
        return;
    m->map_selected = m->map_selected + dir;
    if (m->map_selected < 0)
        m->map_selected = m->map_count - 1;
    if (m->map_selected >= m->map_count)
        m->map_selected = 0;
}

static void leave_map_select(menu_t *m)
{
    cleanup_map_select(m);
    m->screen = SCR_MAIN;
}

static void on_map_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        nav_map(m, -1);
    if (e->key.code == sfKeyDown)
        nav_map(m, 1);
    if (e->key.code == sfKeyReturn)
        confirm_map_selection(m);
    if (e->key.code == sfKeyEscape)
        leave_map_select(m);
}

static void on_map_pad(menu_t *m, sfEvent *e)
{
    int act = pad_menu_action(e);

    if (act == PM_UP)
        nav_map(m, -1);
    if (act == PM_DOWN)
        nav_map(m, 1);
    if (act == PM_OK)
        confirm_map_selection(m);
    if (act == PM_BACK)
        leave_map_select(m);
}

static void on_map_click(menu_t *m, sfEvent *e)
{
    sfVector2f pos = {0};

    pos.x = (float)e->mouseButton.x;
    pos.y = (float)e->mouseButton.y;
    if (button_is_clicked(&m->map_btns[BTN_MAP_PLAY], &pos))
        confirm_map_selection(m);
    if (button_is_clicked(&m->map_btns[BTN_MAP_BACK], &pos))
        leave_map_select(m);
}

void handle_map_events(menu_t *m, sfEvent *e)
{
    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        for (int i = 0; i < MAP_BTN_COUNT; i++)
            update_button(&m->map_btns[i], &m->mouse_pos);
    }
    if (e->type == sfEvtKeyPressed)
        on_map_key(m, e);
    on_map_pad(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft)
        on_map_click(m, e);
}

static void draw_map_bg(menu_t *m, int i, float y)
{
    sfColor sel_bg = {0};
    sfColor nor_bg = {0};
    sfFloatRect bg = {0};

    sel_bg = sfColor_fromRGBA(130, 55, 10, 225);
    nor_bg = sfColor_fromRGBA(15, 8, 4, 185);
    bg.left = (m->ww - 600.0f) / 2.0f - 10.0f;
    bg.top = y - 5.0f;
    bg.width = 620.0f;
    bg.height = 40.0f;
    if (i == m->map_selected)
        draw_filled_rect(m->window, &bg, &sel_bg);
    else
        draw_filled_rect(m->window, &bg, &nor_bg);
}

static void draw_map_label(menu_t *m, int i, float y)
{
    sfText *txt = NULL;
    sfVector2f tpos = {0};

    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, m->map_names[i]);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    if (i == m->map_selected)
        sfText_setFillColor(txt, COL_SEL);
    else
        sfText_setFillColor(txt, sfWhite);
    tpos.x = (m->ww - 600.0f) / 2.0f;
    tpos.y = y;
    sfText_setPosition(txt, tpos);
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

static void draw_map_best(menu_t *m, int i, float y)
{
    sfText *txt = NULL;
    int best = load_best_score(m->map_names[i]);
    char buf[32] = {0};

    if (best <= 0)
        return;
    txt = sfText_create();
    if (!txt)
        return;
    snprintf(buf, sizeof(buf), "BEST %d", best);
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, buf);
    sfText_setCharacterSize(txt, FONT_SMALL_SZ);
    sfText_setFillColor(txt, COL_POP_SECRET);
    sfText_setPosition(txt, (sfVector2f){(m->ww + 600.0f) / 2.0f - 160.0f,
            y + 4.0f});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void render_map_select(menu_t *m)
{
    int i = 0;
    float row_y = 0;

    render_menu_background(m);
    draw_title(m, "SELECT A LEVEL", 60.0f);
    draw_hint(m, "Arrows to navigate, Enter to select", 130.0f);
    if (m->map_count == 0) {
        draw_hint(m, "No levels available", 350.0f);
    } else {
        for (i = 0; i < m->map_count; i++) {
            row_y = 200.0f + i * 50.0f;
            draw_map_bg(m, i, row_y);
            draw_map_label(m, i, row_y);
            draw_map_best(m, i, row_y);
        }
    }
    render_buttons(m->window, m->map_btns, MAP_BTN_COUNT, -1);
}
