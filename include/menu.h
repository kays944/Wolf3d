/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu.h
*/

#ifndef MENU_H_
    #define MENU_H_

    #include "wolf.h"
    #include "button.h"

typedef enum e_menu_screen {
    SCR_MAIN,
    SCR_MAP_SELECT,
    SCR_SETTINGS
} menu_screen_t;

typedef enum e_main_btn {
    BTN_PLAY = 0,
    BTN_MAP,
    BTN_SETTINGS,
    BTN_QUIT,
    MAIN_BTN_COUNT
} main_btn_t;

typedef enum e_map_btn {
    BTN_MAP_PLAY = 0,
    BTN_MAP_BACK,
    MAP_BTN_COUNT
} map_btn_t;

typedef enum e_set_btn {
    BTN_SET_BACK = 0,
    SET_BTN_COUNT
} set_btn_t;

typedef struct s_draw_cfg {
    float x;
    float y;
    unsigned int sz;
    sfColor col;
} draw_cfg_t;

typedef struct s_menu {
    sfRenderWindow *window;
    sfFont *font_big;
    sfFont *font_med;
    sfClock *clock;
    double dt;
    float ww;
    float wh;
    sfVector2f mouse_pos;
    menu_screen_t screen;
    sfBool running;
    int selected;
    button_t main_btns[MAIN_BTN_COUNT];
    sfText *title;
    sfVertexArray *bg;
    button_t map_btns[MAP_BTN_COUNT];
    char map_names[MAX_MAPS][MAP_NAME_LEN];
    int map_count;
    int map_selected;
    button_t set_btns[SET_BTN_COUNT];
    settings_t *settings;
    int settings_sel;
    int action;
    int chosen_map;
} menu_t;

#endif
