/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** flashlight.c
*/

#include <math.h>
#include "macros.h"
#include "proto.h"

static void add_vertex(sfVertexArray *va, float x, float y, const sfColor *c)
{
    sfVertex v = {0};

    v.position.x = x;
    v.position.y = y;
    v.color = *c;
    sfVertexArray_append(va, v);
}

static void build_feather_ring(sfVertexArray *va, float cx, float cy,
    float fl_r)
{
    sfColor dark = sfColor_fromRGBA(0, 0, 0, 210);
    sfColor trans = sfColor_fromRGBA(0, 0, 0, 0);
    float a = 0;

    for (int i = 0; i <= FL_N; i++) {
        a = 2.0f * M_PI * i / FL_N;
        add_vertex(va, cx + fl_r * cosf(a), cy + fl_r * sinf(a), &trans);
        add_vertex(va, cx + (fl_r + FL_FEATHER) * cosf(a),
            cy + (fl_r + FL_FEATHER) * sinf(a), &dark);
    }
}

static void build_dark_ring(sfVertexArray *va, float cx, float cy, float fl_r)
{
    sfColor dark = sfColor_fromRGBA(0, 0, 0, 210);
    float a = 0;

    for (int i = 0; i <= FL_N; i++) {
        a = 2.0f * M_PI * i / FL_N;
        add_vertex(va, cx + (fl_r + FL_FEATHER) * cosf(a),
            cy + (fl_r + FL_FEATHER) * sinf(a), &dark);
        add_vertex(va, cx + FL_R_BIG * cosf(a),
            cy + FL_R_BIG * sinf(a), &dark);
    }
}

static int setup_rings(player_t *p, float cx, float cy)
{
    float fl_r = p->wh / 4.0f;

    p->fl_feather = sfVertexArray_create();
    p->fl_dark = sfVertexArray_create();
    if (!p->fl_feather || !p->fl_dark)
        return EXIT_FAIL;
    sfVertexArray_setPrimitiveType(p->fl_feather, sfTriangleStrip);
    sfVertexArray_setPrimitiveType(p->fl_dark, sfTriangleStrip);
    build_feather_ring(p->fl_feather, cx, cy, fl_r);
    build_dark_ring(p->fl_dark, cx, cy, fl_r);
    return EXIT_SUCCESS;
}

static int load_fl_sprite(player_t *p)
{
    sfVector2u sz = {0};
    sfVector2f sc = {0};
    sfVector2f pos = {0};

    p->fl_tex = sfTexture_createFromFile("./assets/flashlight.png", NULL);
    p->fl_spr = sfSprite_create();
    if (!p->fl_tex || !p->fl_spr)
        return EXIT_FAIL;
    sfSprite_setTexture(p->fl_spr, p->fl_tex, sfTrue);
    sz = sfTexture_getSize(p->fl_tex);
    sc.x = (float)p->ww / 2.0f / sz.x;
    sc.y = sc.x;
    sfSprite_setScale(p->fl_spr, sc);
    pos.x = p->ww - sz.x * sc.x;
    pos.y = p->wh - sz.y * sc.y;
    sfSprite_setPosition(p->fl_spr, pos);
    return EXIT_SUCCESS;
}

int init_flashlight(player_t *p)
{
    float cx = p->ww / 2.0f;
    float cy = p->wh / 2.0f;

    p->fl_feather = NULL;
    p->fl_dark = NULL;
    p->fl_tex = NULL;
    p->fl_spr = NULL;
    if (setup_rings(p, cx, cy) == EXIT_FAIL)
        return EXIT_FAIL;
    if (load_fl_sprite(p) == EXIT_FAIL)
        return EXIT_FAIL;
    p->flashlight = sfFalse;
    return EXIT_SUCCESS;
}

void destroy_flashlight(player_t *p)
{
    if (p->fl_spr)
        sfSprite_destroy(p->fl_spr);
    if (p->fl_tex)
        sfTexture_destroy(p->fl_tex);
    if (p->fl_feather)
        sfVertexArray_destroy(p->fl_feather);
    if (p->fl_dark)
        sfVertexArray_destroy(p->fl_dark);
}

void draw_flashlight(sfRenderWindow *win, player_t *p)
{
    if (!p->flashlight)
        return;
    if (p->fl_spr)
        sfRenderWindow_drawSprite(win, p->fl_spr, NULL);
    sfRenderWindow_drawVertexArray(win, p->fl_feather, NULL);
    sfRenderWindow_drawVertexArray(win, p->fl_dark, NULL);
}

void toggle_flashlight(player_t *p)
{
    p->flashlight = (p->flashlight == sfFalse) ? sfTrue : sfFalse;
}
