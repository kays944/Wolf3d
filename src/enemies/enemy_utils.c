/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy_utils.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

float norm_angle(float a)
{
    while (a > M_PI)
        a -= 2 * M_PI;
    while (a < -M_PI)
        a += 2 * M_PI;
    return a;
}

int has_los(float ex, float ey, player_t *p, map_t *m)
{
    float dx = p->x - ex;
    float dy = p->y - ey;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < 1.0f)
        return 1;
    dx = dx / dist * LOS_STEP;
    dy = dy / dist * LOS_STEP;
    for (float t = 0; t < dist; t += LOS_STEP) {
        if (is_wall(ex, ey, m) == IS_WALL)
            return 0;
        ex += dx;
        ey += dy;
    }
    return 1;
}

static float dist_sq(enemy_t *e, player_t *p)
{
    return (e->x - p->x) * (e->x - p->x) + (e->y - p->y) * (e->y - p->y);
}

static void swap_if_closer(enemy_t **arr, int j, player_t *p)
{
    enemy_t *tmp = NULL;

    if (dist_sq(arr[j], p) >= dist_sq(arr[j + 1], p))
        return;
    tmp = arr[j];
    arr[j] = arr[j + 1];
    arr[j + 1] = tmp;
}

void sort_far(enemy_t **arr, int n, player_t *p)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            swap_if_closer(arr, j, p);
}

static float body_radius(enemy_t *e)
{
    float r = ENEMY_RADIUS * (e->boss ? BOSS_SCALE : 1.0f);

    return r + PLAYER_MARGIN;
}

int blocked_by_enemy(player_t *p, map_t *m, float nx, float ny)
{
    enemy_t *e = NULL;
    float d2 = 0;
    float od2 = 0;
    float r = 0;

    for (int i = 0; i < m->enemy_count; i++) {
        e = &m->enemies[i];
        if (!e->alive || e->dying)
            continue;
        r = body_radius(e);
        d2 = (e->x - nx) * (e->x - nx) + (e->y - ny) * (e->y - ny);
        od2 = (e->x - p->x) * (e->x - p->x)
            + (e->y - p->y) * (e->y - p->y);
        if (d2 < r * r && d2 < od2)
            return 1;
    }
    return 0;
}

static int is_threat(enemy_t *e)
{
    if (!e->alive)
        return 0;
    if (!e->dying)
        return 1;
    return e->death_t < DEATH_FRAMES * DEATH_FRAME_LEN + 0.4f;
}

int enemies_alive(map_t *m)
{
    int count = 0;

    for (int i = 0; i < m->enemy_count; i++)
        if (is_threat(&m->enemies[i]))
            count++;
    return count;
}
