/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** save.c
*/

#include <stdio.h>
#include <unistd.h>
#include "macros.h"
#include "proto.h"

int save_exists(void)
{
    return access(SAVE_PATH, F_OK) == 0;
}

static void write_player(FILE *f, player_t *p)
{
    fprintf(f, "player %.2f %.2f %.4f %d %d %d %d %d %d\n",
        p->x, p->y, p->angle, p->hp, p->ammo, p->reserve,
        p->score, p->kills, p->has_key ? 1 : 0);
}

static void write_enemies(FILE *f, map_t *m)
{
    enemy_t *e = NULL;

    fprintf(f, "night %d %.3f %d\n", m->night ? 1 : 0,
        m->night_cd, m->nights);
    fprintf(f, "enemies %d\n", m->enemy_count);
    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        fprintf(f, "enemy %d %.2f %.2f %d %d %.3f\n", e->type,
            e->x, e->y, e->hp, e->dying ? 1 : 0, e->death_t);
    }
}

static void write_world(FILE *f, map_t *m)
{
    fprintf(f, "packs %d\n", m->pack_count);
    for (int i = 0; i < m->pack_count; i++)
        fprintf(f, "pack %d\n", m->packs[i].active ? 1 : 0);
    fprintf(f, "doors %d\n", m->door_count);
    for (int i = 0; i < m->door_count; i++)
        fprintf(f, "door %d\n", m->doors[i].open ? 1 : 0);
    fprintf(f, "props %d\n", m->prop_count);
    for (int i = 0; i < m->prop_count; i++)
        fprintf(f, "prop %d\n", m->props[i].dead ? 0 : 1);
}

static void write_pwalls(FILE *f, map_t *m)
{
    pwall_t *pw = NULL;

    fprintf(f, "pwalls %d %d\n", m->pwall_count, m->secrets_found);
    for (int i = 0; i < m->pwall_count; i++) {
        pw = &m->pwalls[i];
        fprintf(f, "pwall %d %d %d\n",
            (pw->done || pw->moving) ? 1 : 0, pw->tx, pw->ty);
    }
}

static void write_seen(FILE *f, map_t *m)
{
    if (!m->seen) {
        fprintf(f, "seen 0 0\n");
        return;
    }
    fprintf(f, "seen %d %d\n", m->size_y, m->size_x);
    for (int y = 0; y < m->size_y; y++) {
        for (int x = 0; x < m->size_x; x++)
            fputc(m->seen[y * m->size_x + x] ? '1' : '0', f);
        fputc('\n', f);
    }
}

void save_game(map_t *m, player_t *p)
{
    FILE *f = fopen(SAVE_PATH, "w");

    if (!f) {
        fprintf(stderr, "Warning: could not write %s\n", SAVE_PATH);
        return;
    }
    fprintf(f, "level %s\n", m->path);
    write_player(f, p);
    write_enemies(f, m);
    write_world(f, m);
    write_pwalls(f, m);
    write_seen(f, m);
    fclose(f);
}
