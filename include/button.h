/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** button.h
*/

#ifndef BUTTON_H_
    #define BUTTON_H_

    #include "wolf.h"

typedef struct s_button {
    sfRectangleShape *bg;
    sfText *label;
    sfVector2f pos;
    sfVector2f size;
    int id;
    sfBool hovered;
} button_t;

#endif
