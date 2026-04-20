/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.c
*/

#include "macros.h"
#include "wolf.h"

static sfRenderWindow *creation_window(void)
{
    sfVideoMode mode = {WIN_WIDTH, WIN_HEIGHT, FREQUENCY};
    sfRenderWindow *window;

    window = sfRenderWindow_create(mode, "fen1", sfResize | sfClose, NULL);
    return window;
}

static int game_loop(void)
{
    sfRenderWindow *window = creation_window();

    if (!window)
        return EXIT_FAIL;
    while (sfRenderWindow_isOpen(window)) {
        if (event(window) == EVENT_CLOSE)
            break;
        draw(window);
    }
    close_all(window);
    return EXIT_SUCCESS;
}

int wolf(void)
{
    return game_loop();
}
