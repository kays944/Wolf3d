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

void player_step(player_t *p, map_t *m, float ang, float mag)
{
    float step = PLAYER_SPEED * p->dt * mag;
    float nx = p->x + cosf(ang) * step;
    float ny = p->y + sinf(ang) * step;

    if (is_blocked(nx, ny, m) == IS_WALL
        || is_blocked(nx + PLAYER_MARGIN * cosf(ang),
            ny + PLAYER_MARGIN * sinf(ang), m) == IS_WALL)
        return;
    if (blocked_by_enemy(p, m, nx, ny))
        return;
    p->x = nx;
    p->y = ny;
}

static void move_input(player_t *p, map_t *m)
{
    float fw = 0;
    float side = 0;

    if (sfKeyboard_isKeyPressed(sfKeyZ))
        fw += 1;
    if (sfKeyboard_isKeyPressed(sfKeyS))
        fw -= 1;
    if (sfKeyboard_isKeyPressed(sfKeyD))
        side += 1;
    if (sfKeyboard_isKeyPressed(sfKeyQ))
        side -= 1;
    if (fw == 0 && side == 0)
        return;
    player_step(p, m, p->angle + atan2f(side, fw), 1.0f);
}

static void look_input(player_t *p)
{
    float max_pitch = p->wh / (float)PITCH_MAX_DIV;

    if (!p->use_pad) {
        if (sfKeyboard_isKeyPressed(sfKeyLeft))
            p->angle -= ROTATION_SPEED * p->sens * p->dt;
        if (sfKeyboard_isKeyPressed(sfKeyRight))
            p->angle += ROTATION_SPEED * p->sens * p->dt;
        if (sfKeyboard_isKeyPressed(sfKeyUp))
            p->pitch += PITCH_SPEED * p->dt;
        if (sfKeyboard_isKeyPressed(sfKeyDown))
            p->pitch -= PITCH_SPEED * p->dt;
    }
    if (p->pitch > max_pitch)
        p->pitch = max_pitch;
    if (p->pitch < -max_pitch)
        p->pitch = -max_pitch;
}

static int jump_pressed(player_t *p)
{
    if (p->use_pad)
        return sfJoystick_isConnected(PAD_ID)
            && sfJoystick_isButtonPressed(PAD_ID, PAD_BTN_JUMP);
    return sfKeyboard_isKeyPressed(sfKeySpace);
}

static void update_jump(player_t *player)
{
    if (jump_pressed(player) && player->z <= 0
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

    update_fps(player, dt);
    player->dt = dt < DT_MAX ? dt : DT_MAX;
    if (player->use_pad) {
        update_gamepad(player, m);
    } else {
        move_input(player, m);
    }
    look_input(player);
    update_jump(player);
    update_weapon(player);
    update_reload(player);
}
