/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.h
*/

#ifndef WOLF_H_
    #define WOLF_H_
    #include <SFML/Window.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/System/Vector2.h>
    #include <SFML/Audio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dirent.h>
    #include "macros.h"

typedef struct player_s {
    float x;
    float y;
    float angle;
    int ww;
    int wh;
    int hp;
    float hurt_cd;
    int hurt_flash;
    float z;
    float z_vel;
    float pitch;
    sfText *hp_txt;
    sfBool shot_event;
    sfBool use_pad;
    float *zbuf;
    float dt;
    float sens;
    sfClock *tick_clock;
    sfTexture *enemy_tex;
    sfTexture *boss_tex;
    sfTexture *proj_tex;
    sfSprite *proj_spr;
    sfTexture *pack_tex[PACK_KINDS];
    sfSprite *pack_spr;
    sfTexture *prop_tex[PROP_TYPES];
    sfSprite *prop_spr;
    int ammo;
    int reserve;
    int kills;
    int score;
    sfBool has_key;
    sfBool reached_exit;
    sfText *score_txt;
    int reload_frame;
    sfTexture *weapon_idle;
    sfTexture *weapon_fire;
    sfSprite *weapon_spr;
    sfClock *weapon_clock;
    sfBool firing;
    sfBool flashlight;
    sfTexture *fl_tex;
    sfSprite *fl_spr;
    sfVertexArray *fl_feather;
    sfVertexArray *fl_dark;
    sfTexture *health_tex;
    sfSprite *health_spr;
    sfText *ammo_txt;
    sfFont *hud_font;
    sfTexture *reload_tex;
    sfBool reloading;
    sfClock *reload_clock;
    sfTexture *wall_tex;
    sfImage *wall_img;
    sfImage *sky_img;
    sfUint8 *ceil_pixels;
    sfTexture *ceil_tex;
    sfSprite *ceil_spr;
    sfTexture *floor_tex;
    sfSprite *floor_spr;
} player_t;

typedef struct wall_ctx_s {
    sfVertexArray *va;
    float col_w;
    sfVector2u tex_sz;
    int wh;
    float jr;
    float hy;
} wall_ctx_t;

typedef struct ceil_ctx_s {
    const sfUint8 *wpx;
    sfUint8 *cpx;
    int tw;
    int th;
    int ww;
    float posZ;
    float ldx;
    float ldy;
    float rdx;
    float rdy;
    float px;
    float py;
} ceil_ctx_t;

typedef struct sky_row_s {
    const sfUint8 *spx;
    int tw;
    float u_base;
    float u_step;
    int ww;
} sky_row_t;

typedef struct settings_s {
    float music_vol;
    float sfx_vol;
    int res_index;
    sfBool fullscreen;
    int gamepad;
    float sensitivity;
    int win_w;
    int win_h;
} settings_t;

typedef struct sound_s {
    sfMusic *menu_music;
    sfMusic *game_music;
    sfSoundBuffer *shoot_buf;
    sfSound *shoot_snd;
    sfSound *enemy_snd;
} sound_t;

typedef struct button_s {
    sfRectangleShape *bg;
    sfText *label;
    sfVector2f pos;
    sfVector2f size;
    int id;
    sfBool hovered;
} button_t;

typedef struct enemy_s {
    float x;
    float y;
    int hp;
    int max_hp;
    int type;
    float cooldown;
    float anim_t;
    float atk_anim;
    float death_t;
    float blind;
    sfBool dying;
    sfBool moving;
    sfBool alive;
    sfBool boss;
} enemy_t;

typedef struct pickup_s {
    float x;
    float y;
    int type;
    sfBool active;
} pickup_t;

typedef struct prop_s {
    float x;
    float y;
    int type;
} prop_t;

typedef struct door_s {
    int tx;
    int ty;
    sfBool locked;
    sfBool open;
} door_t;

typedef struct proj_s {
    float x;
    float y;
    float dx;
    float dy;
    sfBool active;
    sfBool boss;
} proj_t;

typedef struct map_s {
    char **map;
    char *path;
    int level;
    int size_x;
    int size_y;
    enemy_t enemies[MAX_ENEMIES];
    int enemy_count;
    proj_t projs[MAX_PROJS];
    pickup_t packs[MAX_PACKS];
    int pack_count;
    prop_t props[MAX_PROPS];
    int prop_count;
    door_t doors[MAX_DOORS];
    int door_count;
    sfBool has_exit;
} map_t;

typedef struct spr_ctx_s {
    sfVertexArray *va;
    sfColor tint;
    sfVector2u tsz;
    float col_w;
    float dist;
    float size;
    float x0;
    float width;
    float ybot;
    float u0;
    float v0;
    float cw;
    float ch;
    int i0;
    int i1;
} spr_ctx_t;

typedef struct end_ctx_s {
    button_t *btns;
    sfText *title;
    sfVertexArray *bg;
    sfClock *clock;
    int n;
    int sel;
} end_ctx_t;

typedef struct pause_s {
    sfRenderWindow *window;
    sfFont *font;
    float ww;
    float wh;
    button_t btns[PAUSE_BTN_COUNT];
    button_t opt_back;
    int screen;
    sfBool running;
    int action;
    int opt_sel;
    sfVector2f mouse;
    sound_t *sound;
    settings_t *settings;
    map_t *map;
    player_t *player;
} pause_t;

typedef struct game_s {
    sfRenderWindow *window;
    sfFont *font_big;
    sfFont *font_med;
    sfBool running;
    game_state_t state;
    settings_t settings;
    sound_t sound;
    map_t map;
} game_t;


int wolf(void);
void draw(sfRenderWindow *window, player_t *player, map_t *m);
int event(sfRenderWindow *window, player_t *player, map_t *m, sound_t *s);
void close_all(sfRenderWindow *window);
int parsing_map(map_t *m, char *path);
void update_player(sfRenderWindow *window, player_t *player, map_t *m);

int is_wall(int x, int y, map_t *m);

typedef struct s_menu {
    sfRenderWindow *window;
    sfFont *font_big;
    sfFont *font_med;
    sfFont *font_title;
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
    sfTexture *bg_tex;
    sfSprite *bg_spr;
    button_t map_btns[MAP_BTN_COUNT];
    char map_names[MAX_MAPS][MAP_NAME_LEN];
    int map_count;
    int map_selected;
    button_t set_btns[SET_BTN_COUNT];
    settings_t *settings;
    sound_t *sound;
    int settings_sel;
    int action;
    int chosen_map;
} menu_t;

#endif
