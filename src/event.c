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
    if (e->type != sfEvtKeyPressed)
        return;
    if (e->key.code != sfKeyP)
        return;
    toggle_flashlight(player);
}

static void check_fire(player_t *player, sfEvent *e, sound_t *s)
{
    if (e->type != sfEvtMouseButtonPressed)
        return;
    if (e->mouseButton.button != sfMouseLeft)
        return;
    if (player->firing == sfTrue)
        return;
    player->firing = sfTrue;
    sfSprite_setTexture(player->weapon_spr, player->weapon_fire, sfTrue);
    sfClock_restart(player->weapon_clock);
    play_shoot(s);
}

int event(sfRenderWindow *window, player_t *player, map_t *m, sound_t *s)
{
    sfEvent ev = {0};

    while (sfRenderWindow_pollEvent(window, &ev)) {
        if (ev.type == sfEvtClosed)
            return EVENT_CLOSE;
        if (ev.type == sfEvtKeyPressed && ev.key.code == sfKeyEscape)
            return EVENT_PAUSE;
        check_fire(player, &ev, s);
        check_switch(player, &ev);
    }
    update_player(window, player, m);
    return EXIT_SUCCESS;
}
