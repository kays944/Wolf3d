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

typedef struct popup_s {
    float x;
    float y;
    float t;
    int amount;
    int kind;
    sfBool active;
} popup_t;

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
    int difficulty;
    float *zbuf;
    float dt;
    float sens;
    sfClock *tick_clock;
    sfTexture *enemy_tex;
    sfTexture *boss_tex;
    sfTexture *runner_tex;
    sfTexture *proj_tex;
    sfSprite *proj_spr;
    sfTexture *pack_tex[PACK_KINDS];
    sfSprite *pack_spr;
    sfTexture *key_tex;
    sfSprite *key_spr;
    sfTexture *prop_tex[PROP_TYPES];
    sfSprite *prop_spr;
    sfTexture *boom_tex;
    sfSprite *boom_spr;
    int ammo;
    int reserve;
    int kills;
    int score;
    sfBool has_key;
    sfBool reached_exit;
    sfBool pickup_event;
    sfBool aiming;
    sfBool r2_down;
    popup_t popups[MAX_POPUPS];
    sfText *score_txt;
    sfText *fps_txt;
    float fps_acc;
    int fps_frames;
    sfBool show_fps;
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
    sfBool reloading;
    sfClock *reload_clock;
    sfTexture *wall_texs[WALL_KINDS];
    sfImage *floor_img;
    sfTexture *sky_tex;
    sfTexture *sky_night_tex;
    sfUint8 *ceil_pixels;
    sfTexture *ceil_tex;
    sfSprite *ceil_spr;
} player_t;

typedef struct wall_ctx_s {
    sfVertexArray *vas[WALL_KINDS];
    float col_w;
    sfVector2u tex_sz[WALL_KINDS];
    int wh;
    float jr;
    float hy;
    struct map_s *m;
    player_t *p;
} wall_ctx_t;

typedef struct wall_hit_s {
    float tex_x;
    int kind;
} wall_hit_t;

typedef struct dda_s {
    float px;
    float py;
    float rdx;
    float rdy;
    float ddx;
    float ddy;
    float sdx;
    float sdy;
    int mx;
    int my;
    int stx;
    int sty;
    int side;
} dda_t;

typedef struct ceil_ctx_s {
    const sfUint32 *fpx;
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

typedef struct settings_s {
    float music_vol;
    float sfx_vol;
    int res_index;
    sfBool fullscreen;
    int gamepad;
    float sensitivity;
    int difficulty;
    int win_w;
    int win_h;
} settings_t;

typedef struct sound_s {
    sfMusic *menu_music;
    sfMusic *night_amb;
    sfMusic *day_amb;
    sfSoundBuffer *shoot_buf;
    sfSound *shoot_snd;
    sfSoundBuffer *reload_buf;
    sfSound *reload_snd;
    sfSoundBuffer *fx_bufs[FX_KINDS];
    float fx_vol[FX_KINDS];
    float fx_pitch[FX_KINDS];
    float fx_jit[FX_KINDS];
    sfSound *pool[SND_POOL];
    int pool_i;
    float sfx_vol;
    float music_vol;
    float amb_vol;
    float day_vol;
    float step_acc;
    float night_fx_cd;
    float last_px;
    float last_py;
    int last_hp;
    int last_res;
    int last_nights;
    int last_step;
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
    sfBool dying;
    sfBool moving;
    sfBool alive;
    sfBool boss;
    sfBool aware;
    float growl_cd;
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
    sfBool dead;
    float fuse;
} prop_t;

typedef struct boom_s {
    float x;
    float y;
    float t;
    sfBool active;
} boom_t;

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
    boom_t booms[MAX_BOOMS];
    door_t doors[MAX_DOORS];
    int door_count;
    sfBool has_exit;
    sfBool night;
    float night_cd;
    int nights;
    int difficulty;
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
    int sel;
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
    sfBool pending_load;
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
