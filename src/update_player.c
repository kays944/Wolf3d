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

static void update_weapon(player_t *player)
{
    sfTime t = {0};
    float ms = 0;

    if (player->firing == sfFalse)
        return;
    t = sfClock_getElapsedTime(player->weapon_clock);
    ms = sfTime_asMilliseconds(t);
    if (ms < 150.0f)
        return;
    player->firing = sfFalse;
    sfSprite_setTexture(player->weapon_spr, player->weapon_idle, sfTrue);
}

void update_player(sfRenderWindow *window, player_t *player, char **map)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ) || sfKeyboard_isKeyPressed(sfKeyS))
        forward_backward(player, map);
    if (sfKeyboard_isKeyPressed(sfKeyQ))
        player->angle -= ROTATION_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyD))
        player->angle += ROTATION_SPEED;
    update_weapon(player);
}
