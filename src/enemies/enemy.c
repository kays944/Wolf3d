/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** enemy.c
*/

#include <time.h>
#include "macros.h"
#include "proto.h"

static void spawn_at(map_t *m, int tx, int ty, sfBool boss)
{
    enemy_t *e = NULL;

    if (m->enemy_count >= MAX_ENEMIES)
        return;
    e = &m->enemies[m->enemy_count];
    e->x = tx * TILE_SIZE + TILE_SIZE / 2;
    e->y = ty * TILE_SIZE + TILE_SIZE / 2;
    e->hp = boss ? BOSS_HP : ENEMY_HP;
    e->cooldown = ENEMY_FIRST_CD_MIN + (rand() % 150) / 100.0f;
    e->anim_t = (rand() % 100) / 100.0f;
    e->atk_anim = 0;
    e->death_t = 0;
    e->dying = sfFalse;
    e->moving = sfFalse;
    e->alive = sfTrue;
    e->boss = boss;
    m->enemy_count++;
}

static void scan_row(map_t *m, int row)
{
    for (int j = 0; m->map[row][j]; j++) {
        if (m->map[row][j] == 'e')
            spawn_at(m, j, row, sfFalse);
        if (m->map[row][j] == 'b')
            spawn_at(m, j, row, sfTrue);
    }
}

int init_enemies(player_t *p, map_t *m)
{
    srand(time(NULL));
    m->enemy_count = 0;
    for (int i = 0; i < m->size_y; i++)
        scan_row(m, i);
    for (int i = 0; i < MAX_PROJS; i++)
        m->projs[i].active = sfFalse;
    p->enemy_tex = sfTexture_createFromFile(ENEMY_TEX_PATH, NULL);
    p->boss_tex = sfTexture_createFromFile(BOSS_TEX_PATH, NULL);
    p->proj_tex = sfTexture_createFromFile(PROJ_TEX_PATH, NULL);
    p->proj_spr = sfSprite_create();
    p->zbuf = malloc(sizeof(float) * NUM_RAYS);
    if (!p->enemy_tex || !p->boss_tex || !p->proj_tex || !p->proj_spr
        || !p->zbuf) {
        destroy_enemies(p);
        return EXIT_FAIL;
    }
    sfSprite_setTexture(p->proj_spr, p->proj_tex, sfTrue);
    for (int i = 0; i < NUM_RAYS; i++)
        p->zbuf[i] = ENEMY_SIGHT;
    return EXIT_SUCCESS;
}

void destroy_enemies(player_t *p)
{
    if (p->proj_spr)
        sfSprite_destroy(p->proj_spr);
    if (p->proj_tex)
        sfTexture_destroy(p->proj_tex);
    if (p->enemy_tex)
        sfTexture_destroy(p->enemy_tex);
    if (p->boss_tex)
        sfTexture_destroy(p->boss_tex);
    if (p->zbuf)
        free(p->zbuf);
    p->proj_spr = NULL;
    p->proj_tex = NULL;
    p->enemy_tex = NULL;
    p->boss_tex = NULL;
    p->zbuf = NULL;
}
