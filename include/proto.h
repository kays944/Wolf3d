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

int pad_menu_action(sfEvent *e);

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
void place_weapon_sprite(player_t *p);

int init_flashlight(player_t *p);
void destroy_flashlight(player_t *p);
void draw_flashlight(sfRenderWindow *win, player_t *p, map_t *m);
void toggle_flashlight(player_t *p);

void init_night(map_t *m);
void update_night(player_t *p, map_t *m);
void draw_night_hud(sfRenderWindow *win, player_t *p, map_t *m);
void spawn_enemy(map_t *m, float x, float y, int type);
void chase(enemy_t *e, player_t *p, map_t *m);
void tick_boss(enemy_t *e, player_t *p, map_t *m, sound_t *s);
void sync_boss_state(enemy_t *e);
void spawn_proj_angle(map_t *m, enemy_t *e, float ang);
void spawn_boom(map_t *m, float x, float y);

int run_pause(game_t *g, map_t *map, player_t *player);
void render_pause(pause_t *p);
void draw_overlay(sfRenderWindow *win, float ww, float wh);
void save_game(map_t *m, player_t *p);
int save_exists(void);
void stash_carry(game_t *g, player_t *p);
void apply_carry(game_t *g, player_t *p);
int load_saved_map(game_t *g);
int apply_save(player_t *p, map_t *m);
void free_map(map_t *m);

int init_sound(sound_t *s, settings_t *set);
void destroy_sound(sound_t *s);
void update_sound_vol(sound_t *s, settings_t *set);
void play_shoot(sound_t *s);
int init_fx(sound_t *s);
void destroy_fx(sound_t *s);
void play_fx(sound_t *s, int id);
sfSound *play_fx_at(sound_t *s, int id, float x, float y);
void update_ambience(player_t *p, map_t *m, sound_t *s);
void reset_ambience(sound_t *s);
void play_reload(sound_t *s);

int init_health_bar(player_t *p);
void destroy_health_bar(player_t *p);
void set_health_frame(player_t *p);

int init_enemies(player_t *p, map_t *m);
void destroy_enemies(player_t *p);
void update_enemies(player_t *p, map_t *m, sound_t *s);
void draw_enemies(sfRenderWindow *win, player_t *p, map_t *m);
void shoot_enemies(player_t *p, map_t *m);
float norm_angle(float a);
int has_los(float ex, float ey, player_t *p, map_t *m);
void hurt_player(player_t *p, int dmg);
void spawn_proj(map_t *m, enemy_t *e, player_t *p);
void update_projs(player_t *p, map_t *m);
void draw_projs(sfRenderWindow *win, player_t *p, map_t *m);

int init_pickups(player_t *p, map_t *m);
void destroy_pickups(player_t *p);
void update_pickups(player_t *p, map_t *m);
void draw_pickups(sfRenderWindow *win, player_t *p, map_t *m);

int is_blocked(float x, float y, map_t *m);
int wall_kind(char c);
void player_step(player_t *p, map_t *m, float ang, float mag);
void update_gamepad(player_t *p, map_t *m);
int init_props(player_t *p, map_t *m);
void destroy_props(player_t *p);
void draw_props(sfRenderWindow *win, player_t *p, map_t *m);
void kill_prop(map_t *m, prop_t *pr);
void shoot_barrels(player_t *p, map_t *m, sound_t *s);
void update_booms(player_t *p, map_t *m, sound_t *s);
int init_booms(player_t *p, map_t *m);
void destroy_booms(player_t *p);
void draw_booms(sfRenderWindow *win, player_t *p, map_t *m);
void sort_far(enemy_t **arr, int n, player_t *p);
int enemies_alive(map_t *m);
int blocked_by_enemy(player_t *p, map_t *m, float nx, float ny);

void draw_hurt_flash(sfRenderWindow *win, player_t *p);
void draw_crosshair(sfRenderWindow *win, player_t *p);
int check_game_end(sfRenderWindow *win, player_t *p, map_t *m);
int run_end_menu(sfRenderWindow *win, player_t *p,
    const char *title, int mode);
int next_level_path(map_t *m, char *buf, int size);
sfVertexArray *make_end_bg(player_t *p, int mode);
void draw_end_scene(sfRenderWindow *win, player_t *p, sfText *t, float el);
void init_end_bg_tex(end_ctx_t *c, int mode);
void draw_end_bg(sfRenderWindow *win, end_ctx_t *c, float ww, float wh);

int init_ammo(player_t *p);
void destroy_ammo(player_t *p);

int init_fps(player_t *p);
void destroy_fps(player_t *p);
void update_fps(player_t *p, float raw_dt);
void draw_fps(sfRenderWindow *win, player_t *p);

int init_score(player_t *p);
void destroy_score(player_t *p);
void add_kill(player_t *p, enemy_t *e, int headshot);
void refresh_score_text(player_t *p);
void draw_score(sfRenderWindow *win, player_t *p);
void draw_minimap(sfRenderWindow *win, player_t *p, map_t *m);

int load_best_score(const char *level_path);
void save_best_score(const char *level_path, int score);
void update_best_score(player_t *p, map_t *m);

void init_doors(map_t *m);
int try_open_door(player_t *p, map_t *m, sound_t *s);
int foe_overlap(map_t *m, enemy_t *self, float nx, float ny);
void try_bite(enemy_t *e, player_t *p, sound_t *s);
void play_death_cry(enemy_t *e, sound_t *s);
void draw_door_hint(sfRenderWindow *win, player_t *p, map_t *m);
void init_keyexit(map_t *m);
void update_keyexit(player_t *p, map_t *m);
void draw_key_hint(sfRenderWindow *win, player_t *p);
void draw_markers(sfRenderWindow *win, player_t *p, map_t *m);

void draw_secrets(sfRenderWindow *win, player_t *p);
void init_pushwalls(map_t *m);
void draw_pushwall_hint(sfRenderWindow *win, player_t *p, map_t *m);
int try_push_wall(player_t *p, map_t *m, sound_t *s);
void update_pushwalls(player_t *p, map_t *m, sound_t *s);
int init_fog(map_t *m);
int fog_seen(map_t *m, int x, int y);
void update_fog(player_t *p, map_t *m);

float diff_hp_mult(int d);
float diff_dmg_mult(int d);
float diff_night_mult(int d);
const char *diff_label(int d);

float world_shade(float dist, map_t *m, player_t *p);
sfColor shade_color(sfColor c, float b);

void spawn_popup(player_t *p, float x, float y, popup_t data);
void update_popups(player_t *p);
void draw_popups(sfRenderWindow *win, player_t *p);
int pickup_in_range(player_t *p, map_t *m);
void draw_pickup_hint(sfRenderWindow *win, player_t *p, map_t *m);
void decrement_ammo(player_t *p);
void reload_ammo(player_t *p);
void refresh_ammo_text(player_t *p);

int init_reload(player_t *p);
void destroy_reload(player_t *p);
void start_reload(player_t *p, sound_t *s);
void update_reload(player_t *p);

int init_wall_tex(player_t *p);
void destroy_wall_tex(player_t *p);
void draw_background(sfRenderWindow *win, player_t *p, map_t *m);
float cast_wall_ray(player_t *p, float angle, map_t *m, wall_hit_t *hit);
float cast_ray_raw(player_t *p, float angle, map_t *m, wall_hit_t *hit);

#endif
