/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** fps.c
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

int init_fps(player_t *p)
{
    p->fps_acc = 0;
    p->fps_frames = 0;
    p->show_fps = sfTrue;
    p->fps_txt = sfText_create();
    if (!p->fps_txt)
        return EXIT_FAIL;
    sfText_setFont(p->fps_txt, p->hud_font);
    sfText_setCharacterSize(p->fps_txt, FPS_FONT_SZ);
    sfText_setFillColor(p->fps_txt, COL_HINT);
    sfText_setPosition(p->fps_txt,
        (sfVector2f){20.0f, p->wh - FPS_FONT_SZ - 18.0f});
    sfText_setString(p->fps_txt, "FPS --");
    return EXIT_SUCCESS;
}

void destroy_fps(player_t *p)
{
    if (p->fps_txt)
        sfText_destroy(p->fps_txt);
    p->fps_txt = NULL;
}

void update_fps(player_t *p, float raw_dt)
{
    char buf[16] = {0};

    p->fps_acc += raw_dt;
    p->fps_frames++;
    if (p->fps_acc < FPS_REFRESH || !p->fps_txt)
        return;
    snprintf(buf, sizeof(buf), "FPS %d",
        (int)(p->fps_frames / p->fps_acc + 0.5f));
    sfText_setString(p->fps_txt, buf);
    p->fps_acc = 0;
    p->fps_frames = 0;
}

void draw_fps(sfRenderWindow *win, player_t *p)
{
    if (p->show_fps && p->fps_txt)
        sfRenderWindow_drawText(win, p->fps_txt, NULL);
}
