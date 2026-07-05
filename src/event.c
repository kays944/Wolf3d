/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** event.c
*/

#include "macros.h"
#include "proto.h"

static void check_switch(player_t *player, sfEvent *e)
{
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code != sfKeyP)
        return;
    toggle_flashlight(player);
}

static void check_reload(player_t *player, sfEvent *e)
{
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code != sfKeyR)
        return;
    start_reload(player);
}

static void do_fire(player_t *player, sound_t *s)
{
    if (player->firing || player->reloading || player->ammo <= 0)
        return;
    player->firing = sfTrue;
    player->shot_event = sfTrue;
    sfSprite_setTexture(player->weapon_spr, player->weapon_fire, sfTrue);
    sfClock_restart(player->weapon_clock);
    decrement_ammo(player);
    play_shoot(s);
}

static void check_fire(player_t *player, sfEvent *e, sound_t *s)
{
    if (player->use_pad || e->type != sfEvtMouseButtonPressed)
        return;
    if (e->mouseButton.button != sfMouseLeft)
        return;
    do_fire(player, s);
}

static void check_pad_buttons(player_t *player, sfEvent *e, sound_t *s)
{
    unsigned int btn = 0;

    if (!player->use_pad || e->type != sfEvtJoystickButtonPressed)
        return;
    btn = e->joystickButton.button;
    if (btn == PAD_BTN_FIRE)
        do_fire(player, s);
    if (btn == PAD_BTN_RELOAD)
        start_reload(player);
    if (btn == PAD_BTN_FLASH)
        toggle_flashlight(player);
}

int event(sfRenderWindow *window, player_t *player, map_t *m, sound_t *s)
{
    sfEvent ev = {0};

    while (sfRenderWindow_pollEvent(window, &ev)) {
        if (ev.type == sfEvtClosed)
            return EVENT_CLOSE;
        if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape)
            return EVENT_PAUSE;
        if (ev.type == sfEvtJoystickButtonPressed
            && ev.joystickButton.button == PAD_BTN_PAUSE)
            return EVENT_PAUSE;
        check_fire(player, &ev, s);
        check_pad_buttons(player, &ev, s);
        check_switch(player, &ev);
        check_reload(player, &ev);
    }
    if (player->shot_event) {
        shoot_enemies(player, m);
        player->shot_event = sfFalse;
    }
    update_player(window, player, m);
    update_enemies(player, m, s);
    update_projs(player, m);
    update_pickups(player, m);
    update_keyexit(player, m);
    update_doors(player, m);
    return EXIT_SUCCESS;
}
