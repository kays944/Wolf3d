/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** map_select_init.c
*/

#include "menu_proto.h"

static int has_wolf_ext(const char *name)
{
    size_t nlen = 0;
    size_t elen = 0;

    nlen = strlen(name);
    elen = strlen(MAP_EXT);
    if (nlen <= elen)
        return 0;
    return strcmp(name + nlen - elen, MAP_EXT) == 0;
}

static int scan_map_files(menu_t *m)
{
    DIR *dir = {0};
    struct dirent *entry = {0};

    m->map_count = 0;
    dir = opendir(MAP_DIR);
    if (!dir)
        return 0;
    entry = readdir(dir);
    while (entry && m->map_count < MAX_MAPS) {
        if (has_wolf_ext(entry->d_name)) {
            strncpy(m->map_names[m->map_count], entry->d_name,
                MAP_NAME_LEN - 1);
            m->map_count++;
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return m->map_count;
}

static void create_map_buttons(menu_t *m)
{
    float cx = 0;
    float by = 0;
    sfVector2f p = {0};

    cx = (m->ww / 2.0f) - BTN_W - BTN_GAP / 2.0f;
    by = m->wh - 120.0f;
    p = (sfVector2f){cx, by};
    init_button(&m->map_btns[BTN_MAP_PLAY], &p, "JOUER", m->font_med);
    p = (sfVector2f){cx + BTN_W + BTN_GAP, by};
    init_button(&m->map_btns[BTN_MAP_BACK], &p, "RETOUR", m->font_med);
    m->map_btns[BTN_MAP_PLAY].id = BTN_MAP_PLAY;
    m->map_btns[BTN_MAP_BACK].id = BTN_MAP_BACK;
}

void init_map_select(menu_t *m)
{
    m->map_selected = 0;
    scan_map_files(m);
    create_map_buttons(m);
}

void cleanup_map_select(menu_t *m)
{
    for (int i = 0; i < MAP_BTN_COUNT; i++)
        destroy_button(&m->map_btns[i]);
}
