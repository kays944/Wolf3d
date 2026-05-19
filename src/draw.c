/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** draw.c
*/

#include "macros.h"
#include "proto.h"
#include <math.h>

int is_wall(int x, int y, char **map)
{
    int tile_x = x / TILE_SIZE;
    int tile_y = y / TILE_SIZE;

    if (tile_x < 0 || tile_x >= MAP_WIDTH
        || tile_y < 0 || tile_y >= MAP_HEIGHT
        || map[tile_y][tile_x] == 'x')
        return IS_WALL;
    return EXIT_SUCCESS;
}

static void draw_floor_and_ceiling(sfRenderWindow *window)
{
    sfRectangleShape *rect = sfRectangleShape_create();

    sfRectangleShape_setSize(rect, (sfVector2f){WIN_WIDTH, WIN_HEIGHT / 2});
    sfRectangleShape_setPosition(rect, (sfVector2f){0, 0});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(50, 50, 50));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){0, WIN_HEIGHT / 2});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(100, 100, 100));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_destroy(rect);
}

static float cast_single_ray(player_t *player, float angle, char **map)
{
    float x = player->x;
    float y = player->y;

    while (is_wall(x, y, map) != IS_WALL) {
        x += cosf(angle) * STEP;
        y += sinf(angle) * STEP;
    }
    return sqrtf((x - player->x) * (x - player->x)
        + (y - player->y) * (y - player->y))
        * cosf(player->angle - angle);
}

static void draw_wall_col(sfRenderWindow *win, sfRectangleShape *rect,
    int ray, float dist)
{
    float wall_h = (TILE_SIZE * WIN_HEIGHT) / dist;
    float draw_h = wall_h < WIN_HEIGHT ? wall_h : WIN_HEIGHT;
    float col_w = WIN_WIDTH / (float)NUM_RAYS;
    sfVector2f size = {col_w, draw_h};
    sfVector2f pos = {ray * col_w, WIN_HEIGHT / 2.0f - draw_h / 2.0f};

    sfRectangleShape_setSize(rect, size);
    sfRectangleShape_setPosition(rect, pos);
    sfRenderWindow_drawRectangleShape(win, rect, NULL);
}

static void cast_all_rays(sfRenderWindow *win, player_t *player, char **map)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    float angle = 0;
    float dist = 0;
    int i = 0;

    if (!rect)
        return;
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(80, 80, 80));
    for (i = 0; i < NUM_RAYS; i++) {
        angle = player->angle - (FOV / 2) + (FOV * i / NUM_RAYS);
        angle = fmodf(angle + 2 * M_PI, 2 * M_PI);
        dist = cast_single_ray(player, angle, map);
        if (dist < DISTANCE_LIMIT)
            dist = DISTANCE_LIMIT;
        draw_wall_col(win, rect, i, dist);
    }
    sfRectangleShape_destroy(rect);
}

void draw(sfRenderWindow *window, player_t *player, char **map)
{
    sfRenderWindow_clear(window, sfBlack);
    draw_floor_and_ceiling(window);
    cast_all_rays(window, player, map);
    if (player->weapon_spr && !player->flashlight)
        sfRenderWindow_drawSprite(window, player->weapon_spr, NULL);
    draw_flashlight(window, player);
    sfRenderWindow_display(window);
}
