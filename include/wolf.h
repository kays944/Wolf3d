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
} player_t;

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

typedef struct s_settings {
    float music_vol;
    float sfx_vol;
    int res_index;
    sfBool fullscreen;
    int win_w;
    int win_h;
} settings_t;

typedef struct s_sound {
    sfMusic *menu_music;
    sfMusic *game_music;
    sfSoundBuffer *shoot_buf;
    sfSound *shoot_snd;
} sound_t;

typedef struct s_button {
    sfRectangleShape *bg;
    sfText *label;
    sfVector2f pos;
    sfVector2f size;
    int id;
    sfBool hovered;
} button_t;

typedef struct s_pause {
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
    char **map;
    player_t *player;
} pause_t;

typedef struct s_game {
    sfRenderWindow *window;
    sfFont *font_big;
    sfFont *font_med;
    sfBool running;
    game_state_t state;
    settings_t settings;
    sound_t sound;
    int selected_map;
} game_t;


int wolf(void);
void draw(sfRenderWindow *window, player_t *player, char **map);
int event(sfRenderWindow *window, player_t *player, char **map, sound_t *s);
void close_all(sfRenderWindow *window);
char **parsing_map(char *path);
void update_player(sfRenderWindow *window, player_t *player, char **map);

int is_wall(int x, int y, char **map);

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
