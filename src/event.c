/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** event.c
*/

#include "macros.h"
#include "wolf.h"

int event(sfRenderWindow *window)
{
    sfEvent event = {0};
    sfVector2i mouse = {0};

    while (sfRenderWindow_pollEvent(window, &event)) {
        mouse = sfMouse_getPositionRenderWindow(window);
        if (event.type == sfEvtClosed)
            return EVENT_CLOSE;
    }
    return EXIT_SUCCESS;
}
