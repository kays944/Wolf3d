/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** assets.c
*/

#include "game_proto.h"

sfFont *load_font_safe(void)
{
    sfFont *font;

    font = sfFont_createFromFile("assets/fonts/wolf3d.ttf");
    if (font)
        return font;
    font = sfFont_createFromFile(
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    if (font)
        return font;
    font = sfFont_createFromFile(
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf");
    if (font)
        return font;
    font = sfFont_createFromFile(
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf");
    if (font)
        return font;
    fprintf(stderr, "Error: no font found\n");
    return NULL;
}

static void set_vert(sfVertex *v, float x, float y, const sfColor *col)
{
    v->position.x = x;
    v->position.y = y;
    v->color = *col;
    v->texCoords.x = 0;
    v->texCoords.y = 0;
}

sfVertexArray *create_gradient_bg(const sfColor *top, const sfColor *bot,
    float w, float h)
{
    sfVertexArray *va;

    va = sfVertexArray_create();
    if (!va)
        return NULL;
    sfVertexArray_setPrimitiveType(va, sfQuads);
    sfVertexArray_resize(va, 4);
    set_vert(sfVertexArray_getVertex(va, 0), 0, 0, top);
    set_vert(sfVertexArray_getVertex(va, 1), w, 0, top);
    set_vert(sfVertexArray_getVertex(va, 2), w, h, bot);
    set_vert(sfVertexArray_getVertex(va, 3), 0, h, bot);
    return va;
}
