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
    int ammo;
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

typedef struct s_wall_ctx {
    sfVertexArray *va;
    float col_w;
    sfVector2u tex_sz;
    int wh;
} wall_ctx_t;

typedef struct s_ceil_ctx {
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

typedef enum e_game_state {
    STATE_MENU,
    STATE_GAME,
    STATE_QUIT
} game_state_t;

typedef enum e_menu_screen {
    SCR_MAIN,
    SCR_MAP_SELECT,
    SCR_SETTINGS
} menu_screen_t;

typedef enum e_main_btn {
    BTN_PLAY = 0,
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

typedef struct settings_s {
    float music_vol;
    float sfx_vol;
    int res_index;
    sfBool fullscreen;
    int win_w;
    int win_h;
} settings_t;

typedef struct sound_s {
    sfMusic *menu_music;
    sfMusic *game_music;
    sfSoundBuffer *shoot_buf;
    sfSound *shoot_snd;
} sound_t;

typedef struct button_s {
    sfRectangleShape *bg;
    sfText *label;
    sfVector2f pos;
    sfVector2f size;
    int id;
    sfBool hovered;
} button_t;

typedef struct map_s {
    char **map;
    char *path;
    int size_x;
    int size_y;
} map_t;

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
