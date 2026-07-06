/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pickup.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static const char PACK_CHARS[PACK_KINDS] = {'h', 'a'};

static const char *PACK_PATHS[PACK_KINDS] = {
    PACK_TEX_PATH,
    AMMO_TEX_PATH,
};

static void scan_pack_row(map_t *m, int row)
{
    for (int j = 0; m->map[row][j]; j++)
        for (int t = 0; t < PACK_KINDS; t++) {
            if (m->map[row][j] != PACK_CHARS[t]
                || m->pack_count >= MAX_PACKS)
                continue;
            m->packs[m->pack_count].x = j * TILE_SIZE + TILE_SIZE / 2;
            m->packs[m->pack_count].y = row * TILE_SIZE + TILE_SIZE / 2;
            m->packs[m->pack_count].type = t;
            m->packs[m->pack_count].active = sfTrue;
            m->pack_count++;
        }
}

int init_pickups(player_t *p, map_t *m)
{
    m->pack_count = 0;
    for (int i = 0; i < m->size_y; i++)
        scan_pack_row(m, i);
    p->pack_spr = sfSprite_create();
    p->key_spr = sfSprite_create();
    p->key_tex = sfTexture_createFromFile(KEY_TEX_PATH, NULL);
    if (!p->pack_spr || !p->key_spr || !p->key_tex) {
        destroy_pickups(p);
        return EXIT_FAIL;
    }
    for (int t = 0; t < PACK_KINDS; t++) {
        p->pack_tex[t] = sfTexture_createFromFile(PACK_PATHS[t], NULL);
        if (!p->pack_tex[t]) {
            destroy_pickups(p);
            return EXIT_FAIL;
        }
    }
    return EXIT_SUCCESS;
}

void destroy_pickups(player_t *p)
{
    if (p->pack_spr)
        sfSprite_destroy(p->pack_spr);
    if (p->key_spr)
        sfSprite_destroy(p->key_spr);
    if (p->key_tex)
        sfTexture_destroy(p->key_tex);
    p->pack_spr = NULL;
    p->key_spr = NULL;
    p->key_tex = NULL;
    for (int t = 0; t < PACK_KINDS; t++) {
        if (p->pack_tex[t])
            sfTexture_destroy(p->pack_tex[t]);
        p->pack_tex[t] = NULL;
    }
}

static int try_collect(player_t *p, pickup_t *pk)
{
    int add = 0;

    if (pk->type == PACK_MEDKIT) {
        if (p->hp >= PLAYER_HP)
            return 0;
        add = p->hp + PACK_HP > PLAYER_HP ? PLAYER_HP - p->hp : PACK_HP;
        p->hp += add;
        set_health_frame(p);
        return add;
    }
    if (p->reserve >= AMMO_RESERVE_MAX)
        return 0;
    add = p->reserve + AMMO_BOX_VALUE > AMMO_RESERVE_MAX
        ? AMMO_RESERVE_MAX - p->reserve : AMMO_BOX_VALUE;
    p->reserve += add;
    refresh_ammo_text(p);
    return add;
}

static void collect_at(player_t *p, pickup_t *pk)
{
    float rel = norm_angle(atan2f(pk->y - p->y, pk->x - p->x) - p->angle);
    int add = try_collect(p, pk);
    popup_t data = {0};

    if (add <= 0)
        return;
    data.kind = pk->type;
    data.amount = add;
    spawn_popup(p, (rel / FOV + 0.5f) * p->ww, p->wh / 2.0f + p->pitch, data);
    pk->active = sfFalse;
}

int pickup_in_range(player_t *p, map_t *m)
{
    float dx = 0;
    float dy = 0;

    for (int i = 0; i < m->pack_count; i++) {
        if (!m->packs[i].active)
            continue;
        dx = m->packs[i].x - p->x;
        dy = m->packs[i].y - p->y;
        if (dx * dx + dy * dy <= PACK_RADIUS * PACK_RADIUS)
            return 1;
    }
    return 0;
}

void update_pickups(player_t *p, map_t *m)
{
    pickup_t *pk = NULL;
    float dx = 0;
    float dy = 0;

    if (!p->pickup_event)
        return;
    p->pickup_event = sfFalse;
    for (int i = 0; i < m->pack_count; i++) {
        pk = &m->packs[i];
        dx = pk->x - p->x;
        dy = pk->y - p->y;
        if (!pk->active || dx * dx + dy * dy > PACK_RADIUS * PACK_RADIUS)
            continue;
        collect_at(p, pk);
    }
}

static void draw_one_pack(sfRenderWindow *win, pickup_t *pk, player_t *p,
    map_t *m)
{
    float dx = pk->x - p->x;
    float dy = pk->y - p->y;
    float rel = norm_angle(atan2f(dy, dx) - p->angle);
    float dist = sqrtf(dx * dx + dy * dy) * cosf(rel);
    float size = 0;
    float ybot = 0;
    float base = 0;
    int ray = (int)((rel / FOV + 0.5f) * NUM_RAYS);
    sfVector2u tsz;

    if (fabsf(rel) > FOV / 2 + 0.3f || dist < 8.0f)
        return;
    if (ray < 0 || ray >= NUM_RAYS || dist >= p->zbuf[ray])
        return;
    base = (TILE_SIZE * p->wh) / dist;
    size = (PACK_WORLD_SIZE * p->wh) / dist;
    ybot = p->wh / 2.0f + p->pitch + base / 2.0f
        + base * (p->z / TILE_SIZE);
    sfSprite_setTexture(p->pack_spr, p->pack_tex[pk->type], sfTrue);
    sfSprite_setColor(p->pack_spr, shade_color(sfWhite,
            world_shade(dist, m, p)));
    tsz = sfTexture_getSize(p->pack_tex[pk->type]);
    sfSprite_setScale(p->pack_spr, (sfVector2f){size / tsz.x,
            size / tsz.y});
    sfSprite_setPosition(p->pack_spr, (sfVector2f){(rel / FOV + 0.5f)
            * p->ww - size / 2.0f, ybot - size});
    sfRenderWindow_drawSprite(win, p->pack_spr, NULL);
}

void draw_pickups(sfRenderWindow *win, player_t *p, map_t *m)
{
    if (!p->pack_spr || !p->zbuf)
        return;
    for (int i = 0; i < m->pack_count; i++)
        if (m->packs[i].active)
            draw_one_pack(win, &m->packs[i], p, m);
}

void draw_pickup_hint(sfRenderWindow *win, player_t *p, map_t *m)
{
    sfText *t = NULL;
    sfFloatRect b = {0};

    if (!pickup_in_range(p, m))
        return;
    t = sfText_create();
    if (!t)
        return;
    sfText_setFont(t, p->hud_font);
    sfText_setString(t, p->use_pad ? "ROND : ramasser" : "F : ramasser");
    sfText_setCharacterSize(t, 26);
    sfText_setFillColor(t, sfColor_fromRGB(235, 235, 210));
    b = sfText_getLocalBounds(t);
    sfText_setPosition(t, (sfVector2f){(p->ww - b.width) / 2.0f - b.left,
            p->wh * 0.60f});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}
