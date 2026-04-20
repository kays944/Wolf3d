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

int wolf(void);
void draw(sfRenderWindow *window);
int event(sfRenderWindow *window);
void close_all(sfRenderWindow *window);

#endif
