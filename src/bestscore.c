/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** bestscore.c
*/

#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "proto.h"

static void level_key(const char *path, char *buf, int size)
{
    const char *base = path ? strrchr(path, '/') : NULL;

    base = base ? base + 1 : path;
    snprintf(buf, size, "%s", base ? base : "unknown");
}

int load_best_score(const char *level_path)
{
    char key[MAP_NAME_LEN] = {0};
    char name[MAP_NAME_LEN] = {0};
    int score = 0;
    FILE *f = fopen(BEST_SCORE_PATH, "r");

    if (!f)
        return 0;
    level_key(level_path, key, sizeof(key));
    while (fscanf(f, "%63s %d", name, &score) == 2)
        if (strcmp(name, key) == 0) {
            fclose(f);
            return score;
        }
    fclose(f);
    return 0;
}

static int read_rows(FILE *f, char rows[][MAP_NAME_LEN], int *scores)
{
    int n = 0;

    while (n < MAX_MAPS && fscanf(f, "%63s %d", rows[n], &scores[n]) == 2)
        n++;
    return n;
}

static int load_rows(char rows[][MAP_NAME_LEN], int *scores)
{
    FILE *f = fopen(BEST_SCORE_PATH, "r");
    int n = 0;

    if (!f)
        return 0;
    n = read_rows(f, rows, scores);
    fclose(f);
    return n;
}

static int find_row(char rows[][MAP_NAME_LEN], int n, const char *key)
{
    for (int i = 0; i < n; i++)
        if (strcmp(rows[i], key) == 0)
            return i;
    return -1;
}

static void write_rows(char rows[][MAP_NAME_LEN], int *scores, int n)
{
    FILE *f = fopen(BEST_SCORE_PATH, "w");

    if (!f)
        return;
    for (int i = 0; i < n; i++)
        fprintf(f, "%s %d\n", rows[i], scores[i]);
    fclose(f);
}

void save_best_score(const char *level_path, int score)
{
    char rows[MAX_MAPS][MAP_NAME_LEN] = {{0}};
    int scores[MAX_MAPS] = {0};
    char key[MAP_NAME_LEN] = {0};
    int n = load_rows(rows, scores);
    int idx = 0;

    level_key(level_path, key, sizeof(key));
    idx = find_row(rows, n, key);
    if (idx < 0 && n < MAX_MAPS) {
        snprintf(rows[n], MAP_NAME_LEN, "%s", key);
        idx = n;
        n++;
    }
    if (idx >= 0 && score > scores[idx])
        scores[idx] = score;
    write_rows(rows, scores, n);
}

void update_best_score(player_t *p, map_t *m)
{
    int best = load_best_score(m->path);

    p->new_best = p->score > best ? sfTrue : sfFalse;
    p->best_score = p->new_best ? p->score : best;
    if (p->new_best)
        save_best_score(m->path, p->score);
}
