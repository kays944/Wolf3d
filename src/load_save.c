/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** load_save.c
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

int load_saved_map(game_t *g)
{
    char path[MAP_NAME_LEN + 32] = {0};
    FILE *f = fopen(SAVE_PATH, "r");

    if (!f)
        return EXIT_FAIL;
    if (fscanf(f, "level %95s\n", path) != 1) {
        fclose(f);
        return EXIT_FAIL;
    }
    fclose(f);
    free_map(&g->map);
    return parsing_map(&g->map, path);
}

static int read_player(FILE *f, player_t *p)
{
    int key = 0;

    if (fscanf(f, "player %f %f %f %d %d %d %d %d %d\n",
        &p->x, &p->y, &p->angle, &p->hp, &p->ammo, &p->reserve,
        &p->score, &p->kills, &key) != 9)
        return EXIT_FAIL;
    p->has_key = key ? sfTrue : sfFalse;
    return EXIT_SUCCESS;
}

static int read_one_enemy(FILE *f, map_t *m, int i)
{
    enemy_t tmp = {0};
    int type = 0;
    int dying = 0;

    if (fscanf(f, "enemy %d %f %f %d %d %f\n", &type,
        &tmp.x, &tmp.y, &tmp.hp, &dying, &tmp.death_t) != 6)
        return EXIT_FAIL;
    if (type < ENEMY_TYPE_GRUNT || type > ENEMY_TYPE_BOSS)
        return EXIT_FAIL;
    spawn_enemy(m, tmp.x, tmp.y, type);
    if (m->enemy_count != i + 1)
        return EXIT_FAIL;
    m->enemies[i].hp = tmp.hp;
    m->enemies[i].dying = dying ? sfTrue : sfFalse;
    m->enemies[i].death_t = tmp.death_t;
    return EXIT_SUCCESS;
}

static int read_enemies(FILE *f, map_t *m)
{
    int n = 0;
    int night = 0;

    n = fscanf(f, "night %d %f %d\n", &night, &m->night_cd, &m->nights);
    if (n < 2)
        return EXIT_FAIL;
    if (n == 2)
        m->nights = night;
    m->night = night ? sfTrue : sfFalse;
    n = 0;
    if (fscanf(f, "enemies %d\n", &n) != 1 || n > MAX_ENEMIES || n < 0)
        return EXIT_FAIL;
    m->enemy_count = 0;
    for (int i = 0; i < n; i++)
        if (read_one_enemy(f, m, i) == EXIT_FAIL)
            return EXIT_FAIL;
    return EXIT_SUCCESS;
}

static int read_world(FILE *f, map_t *m)
{
    int n = 0;
    int v = 0;

    if (fscanf(f, "packs %d\n", &n) != 1 || n != m->pack_count)
        return EXIT_FAIL;
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "pack %d\n", &v) != 1)
            return EXIT_FAIL;
        m->packs[i].active = v ? sfTrue : sfFalse;
    }
    if (fscanf(f, "doors %d\n", &n) != 1 || n != m->door_count)
        return EXIT_FAIL;
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "door %d\n", &v) != 1)
            return EXIT_FAIL;
        if (v)
            m->map[m->doors[i].ty][m->doors[i].tx] = ' ';
        m->doors[i].open = v ? sfTrue : sfFalse;
    }
    return EXIT_SUCCESS;
}

static void strip_key(player_t *p, map_t *m)
{
    if (!p->has_key)
        return;
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++)
            if (m->map[y][x] == 'K')
                m->map[y][x] = ' ';
}

int apply_save(player_t *p, map_t *m)
{
    char skip[MAP_NAME_LEN + 32] = {0};
    FILE *f = fopen(SAVE_PATH, "r");
    int ok = 0;

    if (!f)
        return EXIT_FAIL;
    ok = fscanf(f, "level %95s\n", skip) == 1
        && read_player(f, p) == EXIT_SUCCESS
        && read_enemies(f, m) == EXIT_SUCCESS
        && read_world(f, m) == EXIT_SUCCESS;
    fclose(f);
    if (!ok)
        return EXIT_FAIL;
    strip_key(p, m);
    set_health_frame(p);
    refresh_ammo_text(p);
    refresh_score_text(p);
    return EXIT_SUCCESS;
}
