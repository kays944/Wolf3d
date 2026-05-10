/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** menu_proto.h
*/

#ifndef MENU_PROTO_H_
    #define MENU_PROTO_H_

    #include "menu.h"
    #include "button_proto.h"
    #include "game_proto.h"

int run_menu(menu_t *m);
int init_menu(menu_t *m, game_t *g);
void cleanup_menu(menu_t *m);

void render_menu(menu_t *m);
void render_main_screen(menu_t *m);
void render_background(menu_t *m);
void render_title(menu_t *m);

void handle_menu_events(menu_t *m);
void process_main_event(menu_t *m, sfEvent *e);
void navigate_menu(menu_t *m, int dir);
void confirm_menu_selection(menu_t *m);
void handle_mouse_click(menu_t *m, const sfVector2f *mouse);

int init_map_select(menu_t *m);
void cleanup_map_select(menu_t *m);
void render_map_select(menu_t *m);
void render_map_list(menu_t *m);
void handle_map_events(menu_t *m, sfEvent *e);
void navigate_map_list(menu_t *m, int dir);
void confirm_map_selection(menu_t *m);

int init_settings_menu(menu_t *m);
void cleanup_settings_menu(menu_t *m);
void render_settings(menu_t *m);
void render_volume_bar(menu_t *m, const char *lbl, float val, float y);
void render_res_selector(menu_t *m, float y);
void render_fullscreen_toggle(menu_t *m, float y);
void handle_settings_events(menu_t *m, sfEvent *e);
void adjust_volume(menu_t *m, int which, float delta);
void cycle_resolution(menu_t *m, int dir);
void toggle_fullscreen_setting(menu_t *m);

void draw_text_centered(menu_t *m, const char *str, const draw_cfg_t *cfg);
void draw_text_at(menu_t *m, const char *str, const draw_cfg_t *cfg);
void draw_filled_rect(sfRenderWindow *win,
    const sfFloatRect *r, const sfColor *col);

#endif
