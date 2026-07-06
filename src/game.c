/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** game.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static int find_in_row(char *row, int *r)
{
    int i = 0;

    while (row[i]) {
        if (row[i] == 'o') {
            *r = i;
            return EXIT_SUCCESS;
        }
        i++;
    }
    return EXIT_FAIL;
}

static int find_start(map_t *m, int *col, int *row)
{
    int c = 0;

    while (c < m->size_y) {
        if (find_in_row(m->map[c], row) == EXIT_SUCCESS) {
            *col = c;
            return EXIT_SUCCESS;
        }
        c++;
    }
    return EXIT_FAIL;
}

static int init_player(map_t *m, player_t *player)
{
    int col = 0;
    int row = 0;

    if (find_start(m, &col, &row) == EXIT_FAIL)
        return EXIT_FAIL;
    player->x = row * TILE_SIZE + TILE_SIZE / 2;
    player->y = col * TILE_SIZE + TILE_SIZE / 2;
    player->angle = 0;
    player->hp = PLAYER_HP;
    player->tick_clock = sfClock_create();
    if (!player->tick_clock)
        return EXIT_FAIL;
    return EXIT_SUCCESS;
}

void cleanup_game(game_t *g)
{
    free_map(&g->map);
    destroy_sound(&g->sound);
    if (g->font_big)
        sfFont_destroy(g->font_big);
    if (g->window)
        sfRenderWindow_destroy(g->window);
}

static int init_hud(player_t *player)
{
    if (init_health_bar(player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_ammo(player) == EXIT_FAIL) {
        destroy_health_bar(player);
        return EXIT_FAIL;
    }
    if (init_reload(player) == EXIT_FAIL) {
        destroy_ammo(player);
        destroy_health_bar(player);
        return EXIT_FAIL;
    }
    if (init_wall_tex(player) == EXIT_FAIL) {
        destroy_reload(player);
        destroy_ammo(player);
        destroy_health_bar(player);
        return EXIT_FAIL;
    }
    if (init_score(player) == EXIT_FAIL || init_fps(player) == EXIT_FAIL) {
        destroy_score(player);
        destroy_wall_tex(player);
        destroy_reload(player);
        destroy_ammo(player);
        destroy_health_bar(player);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static int init_player_tools(player_t *player)
{
    if (init_weapon(player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_flashlight(player) == EXIT_FAIL) {
        destroy_weapon(player);
        return EXIT_FAIL;
    }
    if (init_hud(player) == EXIT_FAIL) {
        destroy_flashlight(player);
        destroy_weapon(player);
        return EXIT_FAIL;
    }
    return EXIT_SUCCESS;
}

static int run_game(sfRenderWindow *w, player_t *p, game_t *g)
{
    int ev = 0;
    int ret = PAUSE_RESUME;
    int end = 0;

    sfMusic_stop(g->sound.menu_music);
    reset_ambience(&g->sound);
    while (sfRenderWindow_isOpen(w) && ret == PAUSE_RESUME) {
        ev = event(w, p, &g->map, &g->sound);
        if (ev == EVENT_CLOSE)
            break;
        if (ev == EVENT_PAUSE)
            ret = run_pause(g, &g->map, p);
        if (ret == PAUSE_RESUME)
            end = check_game_end(w, p, &g->map);
        if (end != 0)
            ret = end;
        if (ret == PAUSE_RESUME)
            draw(w, p, &g->map);
    }
    sfMusic_stop(g->sound.night_amb);
    return ret;
}

static void destroy_player_tools(player_t *player)
{
    if (player->tick_clock)
        sfClock_destroy(player->tick_clock);
    destroy_fps(player);
    destroy_score(player);
    destroy_props(player);
    destroy_pickups(player);
    destroy_enemies(player);
    destroy_wall_tex(player);
    destroy_reload(player);
    destroy_ammo(player);
    destroy_health_bar(player);
    destroy_flashlight(player);
    destroy_weapon(player);
}

int game_loop(game_t *g)
{
    sfVector2u sz = sfRenderWindow_getSize(g->window);
    player_t player = {0};
    int ret = 0;

    player.ww = (int)sz.x;
    player.wh = (int)sz.y;
    player.hud_font = g->font_med;
    player.use_pad = g->settings.gamepad ? sfTrue : sfFalse;
    player.sens = g->settings.sensitivity;
    if (init_player(&g->map, &player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_player_tools(&player) == EXIT_FAIL)
        return EXIT_FAIL;
    if (init_enemies(&player, &g->map) == EXIT_FAIL
        || init_pickups(&player, &g->map) == EXIT_FAIL
        || init_props(&player, &g->map) == EXIT_FAIL) {
        destroy_player_tools(&player);
        return EXIT_FAIL;
    }
    init_doors(&g->map);
    init_keyexit(&g->map);
    init_night(&g->map);
    if (g->pending_load) {
        g->pending_load = sfFalse;
        apply_save(&player, &g->map);
    }
    ret = run_game(g->window, &player, g);
    destroy_player_tools(&player);
    return ret;
}
