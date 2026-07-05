/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** draw.c
*/

#include "macros.h"
#include "proto.h"
#include <math.h>

int wall_kind(char c)
{
    if (c == 'x')
        return WALL_STONE;
    if (c == 'm')
        return WALL_BRICK;
    if (c == 'n')
        return WALL_COLD;
    if (c == 'd')
        return WALL_DOOR;
    if (c == 'D')
        return WALL_DOOR_LOCKED;
    return -1;
}

int is_wall(int x, int y, map_t *m)
{
    int tile_x = x / TILE_SIZE;
    int tile_y = y / TILE_SIZE;

    if (tile_x < 0 || tile_x >= m->size_x
        || tile_y < 0 || tile_y >= m->size_y
        || wall_kind(m->map[tile_y][tile_x]) >= 0)
        return IS_WALL;
    return EXIT_SUCCESS;
}

static int hit_wall_kind(float x, float y, map_t *m)
{
    int tx = (int)x / TILE_SIZE;
    int ty = (int)y / TILE_SIZE;
    int kind = 0;

    if (tx < 0 || tx >= m->size_x || ty < 0 || ty >= m->size_y)
        return WALL_STONE;
    kind = wall_kind(m->map[ty][tx]);
    return kind < 0 ? WALL_STONE : kind;
}

static float cast_single_ray(player_t *player, float angle, map_t *m,
    wall_hit_t *hit)
{
    float x = player->x;
    float y = player->y;
    float fx = 0;

    while (is_wall(x, y, m) != IS_WALL) {
        x += cosf(angle) * STEP;
        y += sinf(angle) * STEP;
    }
    hit->kind = hit_wall_kind(x, y, m);
    fx = fmodf(x, (float)TILE_SIZE);
    if (fx < 1.0f || fx > TILE_SIZE - 1.0f)
        hit->tex_x = fmodf(y, (float)TILE_SIZE) / (float)TILE_SIZE;
    else
        hit->tex_x = fx / (float)TILE_SIZE;
    return sqrtf((x - player->x) * (x - player->x)
        + (y - player->y) * (y - player->y))
        * cosf(player->angle - angle);
}

static int init_wall_ctx(wall_ctx_t *ctx, player_t *p)
{
    ctx->col_w = p->ww / (float)NUM_RAYS;
    ctx->wh = p->wh;
    ctx->jr = p->z / (float)TILE_SIZE;
    ctx->hy = p->wh / 2.0f + p->pitch;
    for (int k = 0; k < WALL_KINDS; k++) {
        ctx->tex_sz[k] = sfTexture_getSize(p->wall_texs[k]);
        ctx->vas[k] = sfVertexArray_create();
        if (!ctx->vas[k])
            return EXIT_FAIL;
        sfVertexArray_setPrimitiveType(ctx->vas[k], sfQuads);
    }
    return EXIT_SUCCESS;
}

static void append_vert(sfVertexArray *va, float x, float y, sfVector2f tex)
{
    sfVertex v = {0};

    v.position = (sfVector2f){x, y};
    v.texCoords = tex;
    v.color = sfWhite;
    sfVertexArray_append(va, v);
}

static void fill_wall_quad(wall_ctx_t *ctx, size_t i, float wall_h,
    wall_hit_t *hit)
{
    sfVertexArray *va = ctx->vas[hit->kind];
    sfVector2u tsz = ctx->tex_sz[hit->kind];
    float x0 = i * ctx->col_w;
    float shift = wall_h * ctx->jr;
    float y_top = ctx->hy - wall_h / 2.0f + shift;
    float y_bot = ctx->hy + wall_h / 2.0f + shift;
    float txi = hit->tex_x * (tsz.x - 1);

    append_vert(va, x0, y_top, (sfVector2f){txi, 0});
    append_vert(va, x0 + ctx->col_w, y_top, (sfVector2f){txi + 1, 0});
    append_vert(va, x0 + ctx->col_w, y_bot,
        (sfVector2f){txi + 1, (float)tsz.y});
    append_vert(va, x0, y_bot, (sfVector2f){txi, (float)tsz.y});
}

static void draw_wall_layers(sfRenderWindow *win, player_t *p,
    wall_ctx_t *ctx)
{
    sfRenderStates rs = sfRenderStates_default();

    for (int k = 0; k < WALL_KINDS; k++) {
        if (!ctx->vas[k])
            continue;
        rs.texture = p->wall_texs[k];
        sfRenderWindow_drawVertexArray(win, ctx->vas[k], &rs);
        sfVertexArray_destroy(ctx->vas[k]);
        ctx->vas[k] = NULL;
    }
}

static void cast_all_rays(sfRenderWindow *win, player_t *player, map_t *m)
{
    wall_ctx_t ctx = {0};
    wall_hit_t hit = {0};
    float angle = 0;
    float dist = 0;

    if (init_wall_ctx(&ctx, player) == EXIT_FAIL) {
        draw_wall_layers(win, player, &ctx);
        return;
    }
    for (size_t i = 0; i < NUM_RAYS; i++) {
        angle = fmodf(player->angle - (FOV / 2) + (FOV * i / NUM_RAYS) + 2 *
            M_PI, 2 * M_PI);
        dist = cast_single_ray(player, angle, m, &hit);
        if (dist < DISTANCE_LIMIT)
            dist = DISTANCE_LIMIT;
        player->zbuf[i] = dist;
        fill_wall_quad(&ctx, i, (TILE_SIZE * player->wh) / dist, &hit);
    }
    draw_wall_layers(win, player, &ctx);
}

void draw(sfRenderWindow *window, player_t *player, map_t *m)
{
    sfRenderWindow_clear(window, sfBlack);
    draw_background(window, player);
    cast_all_rays(window, player, m);
    draw_props(window, player, m);
    draw_pickups(window, player, m);
    draw_markers(window, player, m);
    draw_enemies(window, player, m);
    draw_projs(window, player, m);
    draw_flashlight(window, player);
    if (player->weapon_spr)
        sfRenderWindow_drawSprite(window, player->weapon_spr, NULL);
    draw_crosshair(window, player);
    draw_door_hint(window, player, m);
    draw_hurt_flash(window, player);
    if (player->health_spr)
        sfRenderWindow_drawSprite(window, player->health_spr, NULL);
    if (player->hp_txt)
        sfRenderWindow_drawText(window, player->hp_txt, NULL);
    if (player->ammo_txt)
        sfRenderWindow_drawText(window, player->ammo_txt, NULL);
    draw_score(window, player);
    draw_fps(window, player);
    draw_key_hint(window, player);
    draw_minimap(window, player, m);
    sfRenderWindow_display(window);
}
