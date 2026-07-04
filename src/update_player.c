/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** update_player.c
*/

#include "wolf.h"
#include "macros.h"
#include "proto.h"
#include <math.h>

static void move_axis(player_t *p, map_t *m, float ang)
{
    float step = PLAYER_SPEED * p->dt;
    float nx = p->x + cosf(ang) * step;
    float ny = p->y + sinf(ang) * step;

    if (is_blocked(nx, ny, m) == IS_WALL
        || is_blocked(nx + PLAYER_MARGIN * cosf(ang),
            ny + PLAYER_MARGIN * sinf(ang), m) == IS_WALL)
        return;
    p->x = nx;
    p->y = ny;
}

static void move_input(player_t *p, map_t *m)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ))
        move_axis(p, m, p->angle);
    if (sfKeyboard_isKeyPressed(sfKeyS))
        move_axis(p, m, p->angle + M_PI);
    if (sfKeyboard_isKeyPressed(sfKeyQ))
        move_axis(p, m, p->angle - M_PI / 2);
    if (sfKeyboard_isKeyPressed(sfKeyD))
        move_axis(p, m, p->angle + M_PI / 2);
}

static void look_input(player_t *p)
{
    float max_pitch = p->wh / (float)PITCH_MAX_DIV;

    if (sfKeyboard_isKeyPressed(sfKeyLeft))
        p->angle -= ROTATION_SPEED * p->dt;
    if (sfKeyboard_isKeyPressed(sfKeyRight))
        p->angle += ROTATION_SPEED * p->dt;
    if (sfKeyboard_isKeyPressed(sfKeyUp))
        p->pitch += PITCH_SPEED * p->dt;
    if (sfKeyboard_isKeyPressed(sfKeyDown))
        p->pitch -= PITCH_SPEED * p->dt;
    if (p->pitch > max_pitch)
        p->pitch = max_pitch;
    if (p->pitch < -max_pitch)
        p->pitch = -max_pitch;
}

static void update_jump(player_t *player)
{
    if (sfKeyboard_isKeyPressed(sfKeySpace) && player->z <= 0
        && player->z_vel <= 0)
        player->z_vel = JUMP_VEL;
    if (player->z <= 0 && player->z_vel <= 0)
        return;
    player->z += player->z_vel * player->dt;
    player->z_vel -= GRAVITY * player->dt;
    if (player->z <= 0) {
        player->z = 0;
        player->z_vel = 0;
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

void update_player(sfRenderWindow *window, player_t *player, map_t *m)
{
    float dt = sfTime_asSeconds(sfClock_restart(player->tick_clock));

    player->dt = dt < DT_MAX ? dt : DT_MAX;
    move_input(player, m);
    look_input(player);
    update_jump(player);
    update_weapon(player);
    update_reload(player);
}
