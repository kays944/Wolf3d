/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_select.c
*/

#include "proto.h"

static void scan_maps(menu_t *m)
{
    DIR *dir;
    struct dirent *entry;
    size_t elen;

    m->map_count = 0;
    dir = opendir(MAP_DIR);
    if (!dir)
        return;
    elen = strlen(MAP_EXT);
    for (entry = readdir(dir);
        entry && m->map_count < MAX_MAPS;
        entry = readdir(dir)) {
        if (strlen(entry->d_name) > elen
            && !strcmp(entry->d_name + strlen(entry->d_name) - elen, MAP_EXT)) {
            strncpy(m->map_names[m->map_count],
                entry->d_name, MAP_NAME_LEN - 1);
            m->map_count++;
        }
    }
    closedir(dir);
}

int init_map_select(menu_t *m)
{
    sfVector2f p;
    float cx = 0;

    m->map_selected = 0;
    scan_maps(m);
    cx = (m->ww / 2.0f) - BTN_W - BTN_GAP / 2.0f;
    p = (sfVector2f){cx, m->wh - 120.0f};
    init_button(&m->map_btns[BTN_MAP_PLAY], &p, "JOUER", m->font_med);
    p = (sfVector2f){cx + BTN_W + BTN_GAP, m->wh - 120.0f};
    init_button(&m->map_btns[BTN_MAP_BACK], &p, "RETOUR", m->font_med);
    return 0;
}

void cleanup_map_select(menu_t *m)
{
    int i = 0;

    for (i = 0; i < MAP_BTN_COUNT; i++)
        destroy_button(&m->map_btns[i]);
}

static void navigate_map_list(menu_t *m, int dir)
{
    if (m->map_count == 0)
        return;
    m->map_selected = (m->map_selected + dir + m->map_count) % m->map_count;
}

static void confirm_map_selection(menu_t *m)
{
    if (m->map_count == 0)
        return;
    m->chosen_map = m->map_selected;
    m->action = MENU_PLAY;
    m->running = sfFalse;
}

static void back_to_menu(menu_t *m)
{
    cleanup_map_select(m);
    m->screen = SCR_MAIN;
}

static void on_map_key(menu_t *m, sfEvent *e)
{
    if (e->key.code == sfKeyUp)
        navigate_map_list(m, -1);
    if (e->key.code == sfKeyDown)
        navigate_map_list(m, 1);
    if (e->key.code == sfKeyReturn)
        confirm_map_selection(m);
    if (e->key.code == sfKeyEscape)
        back_to_menu(m);
}

void handle_map_events(menu_t *m, sfEvent *e)
{
    sfVector2f pos;
    int i = 0;

    if (e->type == sfEvtMouseMoved) {
        m->mouse_pos.x = (float)e->mouseMove.x;
        m->mouse_pos.y = (float)e->mouseMove.y;
        for (i = 0; i < MAP_BTN_COUNT; i++)
            update_button(&m->map_btns[i], &m->mouse_pos);
    }
    if (e->type == sfEvtKeyPressed)
        on_map_key(m, e);
    if (e->type == sfEvtMouseButtonPressed
        && e->mouseButton.button == sfMouseLeft) {
        pos.x = (float)e->mouseButton.x;
        pos.y = (float)e->mouseButton.y;
        if (button_is_clicked(&m->map_btns[BTN_MAP_PLAY], &pos))
            confirm_map_selection(m);
        if (button_is_clicked(&m->map_btns[BTN_MAP_BACK], &pos))
            back_to_menu(m);
    }
}

static void draw_map_item(menu_t *m, int i, float y)
{
    static const sfColor SELBG = {110, 25, 15, 220};
    static const sfColor NORBG = {30, 30, 30, 180};
    sfText *txt;
    sfFloatRect bg;

    bg = (sfFloatRect){(m->ww - 600.0f) / 2.0f - 10, y - 5, 620, 40};
    draw_filled_rect(m->window, &bg,
        i == m->map_selected ? &SELBG : &NORBG);
    txt = sfText_create();
    if (!txt)
        return;
    sfText_setFont(txt, m->font_med);
    sfText_setString(txt, m->map_names[i]);
    sfText_setCharacterSize(txt, FONT_LABEL_SZ);
    sfText_setFillColor(txt, i == m->map_selected
        ? COL_SEL : sfWhite);
    sfText_setPosition(txt, (sfVector2f){(m->ww - 600.0f) / 2.0f, y});
    sfRenderWindow_drawText(m->window, txt, NULL);
    sfText_destroy(txt);
}

void render_map_select(menu_t *m)
{
    int i = 0;

    if (m->bg)
        sfRenderWindow_drawVertexArray(m->window, m->bg, NULL);
    draw_title(m, "CHOISIR UNE MAP", 60.0f);
    draw_hint(m, "Fleches pour naviguer, Entree pour selectionner", 130.0f);
    if (m->map_count == 0) {
        draw_hint(m, "Aucune map disponible", 350.0f);
    } else {
        for (i = 0; i < m->map_count; i++)
            draw_map_item(m, i, 200.0f + i * 50.0f);
    }
    render_buttons(m->window, m->map_btns, MAP_BTN_COUNT, -1);
}
