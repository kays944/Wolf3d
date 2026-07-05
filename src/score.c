/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** score.c
*/

#include <stdio.h>
#include "macros.h"
#include "proto.h"

void refresh_score_text(player_t *p)
{
    char buf[48] = {0};

    if (!p->score_txt)
        return;
    snprintf(buf, sizeof(buf), "KILLS %d   SCORE %d", p->kills, p->score);
    sfText_setString(p->score_txt, buf);
}

int init_score(player_t *p)
{
    p->kills = 0;
    p->score = 0;
    p->score_txt = sfText_create();
    if (!p->score_txt)
        return EXIT_FAIL;
    sfText_setFont(p->score_txt, p->hud_font);
    sfText_setCharacterSize(p->score_txt, SCORE_FONT_SZ);
    sfText_setFillColor(p->score_txt, COL_TITLE);
    sfText_setPosition(p->score_txt, (sfVector2f){20.0f, 14.0f});
    refresh_score_text(p);
    return EXIT_SUCCESS;
}

void add_kill(player_t *p, enemy_t *e, int headshot)
{
    p->kills += 1;
    p->score += e->boss ? SCORE_BOSS : SCORE_ENEMY;
    if (headshot)
        p->score += SCORE_HEADSHOT;
    refresh_score_text(p);
}

void draw_score(sfRenderWindow *win, player_t *p)
{
    if (p->score_txt)
        sfRenderWindow_drawText(win, p->score_txt, NULL);
}

void destroy_score(player_t *p)
{
    if (p->score_txt)
        sfText_destroy(p->score_txt);
    p->score_txt = NULL;
}
