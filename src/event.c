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
    if (e->type == sfEvtKeyPressed && e->key.code == sfKeyF3)
        player->show_fps = !player->show_fps;
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code == sfKeyT)
        switch_weapon(player);
    if (e->key.code != sfKeyP)
        return;
    toggle_flashlight(player);
}

static void check_reload(player_t *player, sfEvent *e, sound_t *s)
{
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code != sfKeyR)
        return;
    start_reload(player, s);
}

static void check_open(player_t *player, sfEvent *e, map_t *m, sound_t *s)
{
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code != sfKeyE)
        return;
    if (!try_open_door(player, m, s))
        try_push_wall(player, m, s);
}

static void check_pickup(player_t *player, sfEvent *e)
{
    if (player->use_pad || e->type != sfEvtKeyPressed)
        return;
    if (e->key.code == sfKeyF)
        player->pickup_event = sfTrue;
}

static void do_fire(player_t *player, sound_t *s)
{
    if (player->firing || player->reloading)
        return;
    if (player->weapon == WEAPON_RIFLE && player->ammo <= 0)
        return;
    player->firing = sfTrue;
    player->shot_event = sfTrue;
    sfSprite_setTexture(player->weapon_spr, weapon_fire_tex(player), sfTrue);
    sfClock_restart(player->weapon_clock);
    if (player->weapon == WEAPON_RIFLE) {
        decrement_ammo(player);
        play_shoot(s);
        return;
    }
    play_pistol(s);
}

static void check_fire(player_t *player, sfEvent *e, sound_t *s)
{
    if (player->use_pad || e->type != sfEvtMouseButtonPressed)
        return;
    if (e->mouseButton.button != sfMouseLeft)
        return;
    do_fire(player, s);
}

static void check_pad_buttons(player_t *player, sfEvent *e, sound_t *s,
    map_t *m)
{
    unsigned int btn = 0;

    if (!player->use_pad || e->type != sfEvtJoystickButtonPressed)
        return;
    btn = e->joystickButton.button;
    if (btn == PAD_BTN_FIRE)
        do_fire(player, s);
    if (btn == PAD_BTN_RELOAD && !try_open_door(player, m, s)
        && !try_push_wall(player, m, s))
        start_reload(player, s);
    if (btn == PAD_BTN_FLASH)
        toggle_flashlight(player);
    if (btn == PAD_BTN_PICKUP)
        player->pickup_event = sfTrue;
    if (btn == PAD_BTN_SWAP)
        switch_weapon(player);
}

static void check_triggers(player_t *player, sound_t *s)
{
    float l2 = 0;
    float r2 = 0;

    if (!player->use_pad) {
        player->aiming = sfMouse_isButtonPressed(sfMouseRight);
        return;
    }
    if (!sfJoystick_isConnected(PAD_ID))
        return;
    l2 = sfJoystick_getAxisPosition(PAD_ID, PAD_AIM_AXIS);
    r2 = sfJoystick_getAxisPosition(PAD_ID, PAD_FIRE_AXIS);
    player->aiming = l2 > TRIG_ON ? sfTrue : sfFalse;
    if (r2 > TRIG_ON && !player->r2_down) {
        player->r2_down = sfTrue;
        do_fire(player, s);
    }
    if (r2 < TRIG_OFF)
        player->r2_down = sfFalse;
}

static void run_updates(sfRenderWindow *window, player_t *player,
    map_t *m, sound_t *s)
{
    check_triggers(player, s);
    if (player->shot_event) {
        shoot_enemies(player, m);
        shoot_barrels(player, m, s);
        player->shot_event = sfFalse;
    }
    update_player(window, player, m);
    update_enemies(player, m, s);
    update_projs(player, m);
    update_booms(player, m, s);
    update_pickups(player, m);
    update_popups(player);
    update_pushwalls(player, m, s);
    update_fog(player, m);
    update_keyexit(player, m);
    update_night(player, m);
    update_ambience(player, m, s);
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
        if (ev.type == sfEvtMouseWheelScrolled && !player->use_pad)
            switch_weapon(player);
        check_fire(player, &ev, s);
        check_pad_buttons(player, &ev, s, m);
        check_switch(player, &ev);
        check_reload(player, &ev, s);
        check_open(player, &ev, m, s);
        check_pickup(player, &ev);
    }
    run_updates(window, player, m, s);
    return EXIT_SUCCESS;
}
