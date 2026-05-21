/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** proto.h
*/

#ifndef PROTO_H_
    #define PROTO_H_

    #include "wolf.h"

extern const int RES_W[NUM_RES];
extern const int RES_H[NUM_RES];

int wolf(void);
sfVertexArray *create_gradient_bg(const sfColor *top, const sfColor *bot,
    float w, float h);
void get_resolution(int idx, int *w, int *h);
void save_settings(settings_t *s);

void setup_title_position(menu_t *m);
int init_menu(menu_t *m, game_t *g);
void cleanup_menu(menu_t *m);
int run_menu(menu_t *m);

void render_menu(menu_t *m);
void render_menu_background(menu_t *m);
void draw_filled_rect(sfRenderWindow *win,
    const sfFloatRect *r, const sfColor *col);
void draw_title(menu_t *m, const char *str, float y);
void draw_hint(menu_t *m, const char *str, float y);

int init_map_select(menu_t *m);
void cleanup_map_select(menu_t *m);
void handle_map_events(menu_t *m, sfEvent *e);
void render_map_select(menu_t *m);

int init_settings_menu(menu_t *m);
void cleanup_settings_menu(menu_t *m);
void handle_settings_events(menu_t *m, sfEvent *e);
void render_settings(menu_t *m);

int init_button(button_t *btn, const sfVector2f *pos,
    const char *txt, sfFont *font);
void destroy_button(button_t *btn);
void update_button(button_t *btn, const sfVector2f *mouse);
sfBool button_is_clicked(button_t *btn, const sfVector2f *mouse);
void render_buttons(sfRenderWindow *win, button_t *btns,
    int count, int selected);
int game_init(game_t *g);
sfRenderWindow *open_fullscreen(void);
int game_loop(game_t *g);
void cleanup_game(game_t *g);
int init_weapon(player_t *p);
void destroy_weapon(player_t *p);

int init_flashlight(player_t *p);
void destroy_flashlight(player_t *p);
void draw_flashlight(sfRenderWindow *win, player_t *p);
void toggle_flashlight(player_t *p);

int run_pause(game_t *g, map_t *map, player_t *player);
void render_pause(pause_t *p);
void draw_overlay(sfRenderWindow *win, float ww, float wh);
void saving(char **map, player_t *player);
void free_map(map_t *m);

int init_sound(sound_t *s, settings_t *set);
void destroy_sound(sound_t *s);
void update_sound_vol(sound_t *s, settings_t *set);
void play_shoot(sound_t *s);

int init_health_bar(player_t *p);
void destroy_health_bar(player_t *p);

int init_ammo(player_t *p);
void destroy_ammo(player_t *p);
void decrement_ammo(player_t *p);

#endif
