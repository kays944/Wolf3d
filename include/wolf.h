/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.h
*/

#ifndef WOLF_H_
    #define WOLF_H_

    #include <SFML/Graphics.h>
    #include <SFML/Audio.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include <dirent.h>
    #include "macros.h"

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

#endif
