/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** update_player.c
*/

#include "wolf.h"
#include "macros.h"
#include <math.h>

static void forward_backward(player_t *player, char **map)
{
    float new_x = 0;
    float new_y = 0;

    if (sfKeyboard_isKeyPressed(sfKeyZ)) {
        new_x = player->x + cos(player->angle) * PLAYER_SPEED;
        new_y = player->y + sin(player->angle) * PLAYER_SPEED;
    }
    if (sfKeyboard_isKeyPressed(sfKeyS)) {
        new_x = player->x - cos(player->angle) * PLAYER_SPEED;
        new_y = player->y - sin(player->angle) * PLAYER_SPEED;
    }
    if (is_wall(new_x, new_y, map) != IS_WALL) {
        player->x = new_x;
        player->y = new_y;
    }
}

void update_player(sfRenderWindow *window, player_t *player, char **map)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ) || sfKeyboard_isKeyPressed(sfKeyS))
        forward_backward(player, map);
    if (sfKeyboard_isKeyPressed(sfKeyQ))
        player->angle -= ROTATION_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyD))
        player->angle += ROTATION_SPEED;
}
