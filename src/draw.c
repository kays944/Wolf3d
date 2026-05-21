/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** draw.c
*/

#include "macros.h"
#include "proto.h"
#include <math.h>

int is_wall(int x, int y, map_t *m)
{
    int tile_x = x / TILE_SIZE;
    int tile_y = y / TILE_SIZE;

    if (tile_x < 0 || tile_x >= m->size_x
        || tile_y < 0 || tile_y >= m->size_y
        || m->map[tile_y][tile_x] == 'x')
        return IS_WALL;
    return EXIT_SUCCESS;
}

static void draw_floor_and_ceiling(sfRenderWindow *window, int ww, int wh)
{
    sfRectangleShape *rect = sfRectangleShape_create();

    sfRectangleShape_setSize(rect, (sfVector2f){ww, wh / 2});
    sfRectangleShape_setPosition(rect, (sfVector2f){0, 0});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(50, 50, 50));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){0, wh / 2});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(100, 100, 100));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_destroy(rect);
}

static float cast_single_ray(player_t *player, float angle, map_t *m)
{
    float x = player->x;
    float y = player->y;

    while (is_wall(x, y, m) != IS_WALL) {
        x += cosf(angle) * STEP;
        y += sinf(angle) * STEP;
    }
    return sqrtf((x - player->x) * (x - player->x)
        + (y - player->y) * (y - player->y))
        * cosf(player->angle - angle);
}

static void draw_wall_col(sfRenderWindow *win, sfRectangleShape *rect,
    const sfVector2f *pos, const sfVector2f *size)
{
    sfRectangleShape_setSize(rect, *size);
    sfRectangleShape_setPosition(rect, *pos);
    sfRenderWindow_drawRectangleShape(win, rect, NULL);
}

static sfVector2f wall_draw_height(float dist, int wh, float col_w)
{
    sfVector2f size = {0};
    float wall_h = (TILE_SIZE * wh) / dist;

    if (wall_h < wh)
        size = (sfVector2f){col_w, wall_h};
    else
        size = (sfVector2f){col_w, wh};
    return size;
}

static void cast_all_rays(sfRenderWindow *win, player_t *player, map_t *m)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    sfVector2f size = {0};
    sfVector2f pos = {0};
    float col_w = player->ww / (float)NUM_RAYS;
    float angle = 0;
    float dist = 0;

    if (!rect)
        return;
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(80, 80, 80));
    for (size_t i = 0; i < NUM_RAYS; i++) {
        angle = fmodf(player->angle - (FOV / 2) + (FOV * i / NUM_RAYS) + 2 *
            M_PI, 2 * M_PI);
        dist = (dist < DISTANCE_LIMIT) ? DISTANCE_LIMIT :
            cast_single_ray(player, angle, m);
        size = wall_draw_height(dist, player->wh, col_w);
        pos = (sfVector2f){i * col_w, player->wh / 2.0f - size.y / 2.0f};
        draw_wall_col(win, rect, &pos, &size);
    }
    sfRectangleShape_destroy(rect);
}

void draw(sfRenderWindow *window, player_t *player, map_t *m)
{
    sfRenderWindow_clear(window, sfBlack);
    draw_floor_and_ceiling(window, player->ww, player->wh);
    cast_all_rays(window, player, m);
    if (player->weapon_spr && !player->flashlight)
        sfRenderWindow_drawSprite(window, player->weapon_spr, NULL);
    draw_flashlight(window, player);
    if (player->health_spr)
        sfRenderWindow_drawSprite(window, player->health_spr, NULL);
    sfRenderWindow_display(window);
}
