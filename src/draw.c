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

static float cast_single_ray(player_t *player, float angle, map_t *m,
    float *tex_x)
{
    float x = player->x;
    float y = player->y;
    float fx = 0;

    while (is_wall(x, y, m) != IS_WALL) {
        x += cosf(angle) * STEP;
        y += sinf(angle) * STEP;
    }
    fx = fmodf(x, (float)TILE_SIZE);
    if (fx < 1.0f || fx > TILE_SIZE - 1.0f)
        *tex_x = fmodf(y, (float)TILE_SIZE) / (float)TILE_SIZE;
    else
        *tex_x = fx / (float)TILE_SIZE;
    return sqrtf((x - player->x) * (x - player->x)
        + (y - player->y) * (y - player->y))
        * cosf(player->angle - angle);
}

static int init_wall_ctx(wall_ctx_t *ctx, player_t *p)
{
    ctx->col_w = p->ww / (float)NUM_RAYS;
    ctx->wh = p->wh;
    ctx->tex_sz = sfTexture_getSize(p->wall_tex);
    ctx->va = sfVertexArray_create();
    if (!ctx->va)
        return EXIT_FAIL;
    sfVertexArray_setPrimitiveType(ctx->va, sfQuads);
    sfVertexArray_resize(ctx->va, NUM_RAYS * 4);
    return EXIT_SUCCESS;
}

static void fill_wall_quad(wall_ctx_t *ctx, size_t i, float wall_h, float tx)
{
    sfVertex *v = sfVertexArray_getVertex(ctx->va, i * 4);
    float x0 = i * ctx->col_w;
    float y_top = ctx->wh / 2.0f - wall_h / 2.0f;
    float y_bot = ctx->wh / 2.0f + wall_h / 2.0f;
    float txi = tx * (ctx->tex_sz.x - 1);

    v[0].position = (sfVector2f){x0, y_top};
    v[0].texCoords = (sfVector2f){txi, 0};
    v[0].color = sfWhite;
    v[1].position = (sfVector2f){x0 + ctx->col_w, y_top};
    v[1].texCoords = (sfVector2f){txi + 1, 0};
    v[1].color = sfWhite;
    v[2].position = (sfVector2f){x0 + ctx->col_w, y_bot};
    v[2].texCoords = (sfVector2f){txi + 1, ctx->tex_sz.y};
    v[2].color = sfWhite;
    v[3].position = (sfVector2f){x0, y_bot};
    v[3].texCoords = (sfVector2f){txi, ctx->tex_sz.y};
    v[3].color = sfWhite;
}

static void cast_all_rays(sfRenderWindow *win, player_t *player, map_t *m)
{
    wall_ctx_t ctx = {0};
    sfRenderStates rs = sfRenderStates_default();
    float angle = 0;
    float dist = 0;
    float tex_x = 0;

    if (init_wall_ctx(&ctx, player) == EXIT_FAIL)
        return;
    for (size_t i = 0; i < NUM_RAYS; i++) {
        angle = fmodf(player->angle - (FOV / 2) + (FOV * i / NUM_RAYS) + 2 *
            M_PI, 2 * M_PI);
        dist = cast_single_ray(player, angle, m, &tex_x);
        if (dist < DISTANCE_LIMIT)
            dist = DISTANCE_LIMIT;
        fill_wall_quad(&ctx, i, (TILE_SIZE * player->wh) / dist, tex_x);
    }
    rs.texture = player->wall_tex;
    sfRenderWindow_drawVertexArray(win, ctx.va, &rs);
    sfVertexArray_destroy(ctx.va);
}

void draw(sfRenderWindow *window, player_t *player, map_t *m)
{
    sfRenderWindow_clear(window, sfBlack);
    draw_floor_tex(window, player);
    draw_ceil_tex(window, player);
    cast_all_rays(window, player, m);
    if (player->weapon_spr && !player->flashlight)
        sfRenderWindow_drawSprite(window, player->weapon_spr, NULL);
    draw_flashlight(window, player);
    if (player->health_spr)
        sfRenderWindow_drawSprite(window, player->health_spr, NULL);
    if (player->ammo_txt)
        sfRenderWindow_drawText(window, player->ammo_txt, NULL);
    sfRenderWindow_display(window);
}
