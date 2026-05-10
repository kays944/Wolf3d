/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game_proto.h
*/

#ifndef GAME_PROTO_H_
    #define GAME_PROTO_H_

    #include "wolf.h"

int wolf(void);
int game_init(game_t *g);
void game_cleanup(game_t *g);
sfFont *load_font_safe(void);
sfVertexArray *create_gradient_bg(const sfColor *top, const sfColor *bot,
    float w, float h);
void default_settings(settings_t *s);
int load_settings(settings_t *s);
void save_settings(settings_t *s);
void get_resolution(int idx, int *w, int *h);

#endif
