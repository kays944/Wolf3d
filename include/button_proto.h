/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button_proto.h
*/

#ifndef BUTTON_PROTO_H_
    #define BUTTON_PROTO_H_

    #include "button.h"

int init_button(button_t *btn, const sfVector2f *pos,
    const char *txt, sfFont *font);
void destroy_button(button_t *btn);
void update_button(button_t *btn, const sfVector2f *mouse);
sfBool button_is_clicked(button_t *btn, const sfVector2f *mouse);
void render_button(sfRenderWindow *win, button_t *btn, sfBool sel);
void render_buttons(sfRenderWindow *win, button_t *btns,
    int count, int selected);

#endif
