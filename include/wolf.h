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
    #include <math.h>
    #include <dirent.h>
    #include "macros.h"

typedef struct player_s {
    float x;
    float y;
    float angle;
} player_t;

typedef enum e_game_state {
    STATE_MENU,
    STATE_GAME,
    STATE_QUIT
} game_state_t;

typedef struct s_settings {
    float music_vol;
    float sfx_vol;
    int res_index;
    sfBool fullscreen;
    int win_w;
    int win_h;
} settings_t;

typedef struct s_game {
    sfRenderWindow *window;
    sfFont *font_big;
    sfFont *font_med;
    sfBool running;
    game_state_t state;
    settings_t settings;
    int selected_map;
} game_t;

int wolf(void);
void draw(sfRenderWindow *window, player_t *player, char **map);
int event(sfRenderWindow *window, player_t *player, char **map);
void close_all(sfRenderWindow *window);
char **parsing_map(char *path);
void update_player(sfRenderWindow *window, player_t *player, char **map);

int is_wall(int x, int y, char **map);

#endif
