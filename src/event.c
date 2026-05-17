/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** event.c
*/

#include "macros.h"
#include "proto.h"

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

int event(sfRenderWindow *window, player_t *player, char **map, sound_t *s)
{
    sfEvent ev = {0};

    while (sfRenderWindow_pollEvent(window, &ev)) {
        if (ev.type == sfEvtClosed)
            return EVENT_CLOSE;
        check_fire(player, &ev, s);
    }
    update_player(window, player, map);
    return EXIT_SUCCESS;
}
