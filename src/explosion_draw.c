/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** explosion_draw.c — explosion billboard animation
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

int init_booms(player_t *p, map_t *m)
{
    for (int i = 0; i < MAX_BOOMS; i++)
        m->booms[i].active = sfFalse;
    p->boom_spr = sfSprite_create();
    p->boom_tex = sfTexture_createFromFile(BOOM_TEX_PATH, NULL);
    if (!p->boom_spr || !p->boom_tex) {
        destroy_booms(p);
        return EXIT_FAIL;
    }
    sfSprite_setTexture(p->boom_spr, p->boom_tex, sfTrue);
    return EXIT_SUCCESS;
}

void destroy_booms(player_t *p)
{
    if (p->boom_spr)
        sfSprite_destroy(p->boom_spr);
    p->boom_spr = NULL;
    if (p->boom_tex)
        sfTexture_destroy(p->boom_tex);
    p->boom_tex = NULL;
}

static void place_boom(player_t *p, boom_t *b, float rel, float dist)
{
    float size = (BOOM_WORLD_SIZE * p->wh) / dist;
    float base = (TILE_SIZE * p->wh) / dist;
    float ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    int frame = (int)(b->t / BOOM_FRAME_LEN);

    if (frame >= BOOM_FRAMES)
        frame = BOOM_FRAMES - 1;
    sfSprite_setTextureRect(p->boom_spr, (sfIntRect){frame * BOOM_FRAME_W,
        0, BOOM_FRAME_W, BOOM_FRAME_H});
    sfSprite_setScale(p->boom_spr, (sfVector2f){size / BOOM_FRAME_W,
        size / BOOM_FRAME_H});
    sfSprite_setPosition(p->boom_spr, (sfVector2f){(rel / FOV + 0.5f)
        * p->ww - size / 2.0f, ybot - size});
}

static void draw_one_boom(sfRenderWindow *win, boom_t *b, player_t *p)
{
    float dx = b->x - p->x;
    float dy = b->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    place_boom(p, b, rel, dist);
    sfRenderWindow_drawSprite(win, p->boom_spr, NULL);
}

void draw_booms(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (!p->boom_spr || !p->boom_tex || !p->zbuf)
        return;
    for (int i = 0; i < MAX_BOOMS; i++)
        if (m->booms[i].active)
            draw_one_boom(win, &m->booms[i], p);
}
