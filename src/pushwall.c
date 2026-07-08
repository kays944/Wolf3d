/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** pushwall.c — secret push walls ('s' in maps, slide until blocked)
*/

#include <math.h>
#include <string.h>
#include "macros.h"
#include "proto.h"

void init_pushwalls(map_t *m)
{
    m->pwall_count = 0;
    m->secrets_found = 0;
    for (int y = 0; y < m->size_y; y++)
        for (int x = 0; m->map[y][x]; x++) {
            if (m->map[y][x] != 's' || m->pwall_count >= MAX_PWALLS)
                continue;
            m->pwalls[m->pwall_count].tx = x;
            m->pwalls[m->pwall_count].ty = y;
            m->pwalls[m->pwall_count].moving = sfFalse;
            m->pwalls[m->pwall_count].done = sfFalse;
            m->pwall_count++;
        }
}

static pwall_t *pwall_near(player_t *p, map_t *m)
{
    pwall_t *best = NULL;
    float best_d = PW_RANGE;
    float cx = 0;
    float cy = 0;
    float d = 0;

    for (int i = 0; i < m->pwall_count; i++) {
        if (m->pwalls[i].moving || m->pwalls[i].done)
            continue;
        cx = m->pwalls[i].tx * TILE_SIZE + TILE_SIZE / 2.0f;
        cy = m->pwalls[i].ty * TILE_SIZE + TILE_SIZE / 2.0f;
        d = sqrtf((cx - p->x) * (cx - p->x) + (cy - p->y) * (cy - p->y));
        if (d < best_d) {
            best_d = d;
            best = &m->pwalls[i];
        }
    }
    return best;
}

static void push_dir(player_t *p, pwall_t *pw)
{
    float dx = pw->tx * TILE_SIZE + TILE_SIZE / 2.0f - p->x;
    float dy = pw->ty * TILE_SIZE + TILE_SIZE / 2.0f - p->y;

    pw->dx = 0;
    pw->dy = 0;
    if (fabsf(dx) > fabsf(dy))
        pw->dx = dx > 0 ? 1 : -1;
    else
        pw->dy = dy > 0 ? 1 : -1;
}

static int next_free(map_t *m, pwall_t *pw)
{
    int nx = pw->tx + pw->dx;
    int ny = pw->ty + pw->dy;

    if (ny < 0 || ny >= m->size_y)
        return 0;
    if (nx < 0 || nx >= (int)strlen(m->map[ny]))
        return 0;
    return m->map[ny][nx] == ' ';
}

static void step_pwall(map_t *m, pwall_t *pw)
{
    if (!next_free(m, pw)) {
        m->map[pw->ty][pw->tx] = 'x';
        pw->moving = sfFalse;
        pw->done = sfTrue;
        return;
    }
    m->map[pw->ty][pw->tx] = ' ';
    pw->tx += pw->dx;
    pw->ty += pw->dy;
    m->map[pw->ty][pw->tx] = 's';
}

static void pwall_thunk(sound_t *s, pwall_t *pw)
{
    sfSound *snd = play_fx_at(s, FX_DOOR,
        pw->tx * TILE_SIZE + TILE_SIZE / 2.0f,
        pw->ty * TILE_SIZE + TILE_SIZE / 2.0f);

    if (snd)
        sfSound_setPitch(snd, PW_SND_PITCH);
}

static void reward_secret(player_t *p, map_t *m, pwall_t *pw)
{
    popup_t data = {0};

    m->secrets_found++;
    p->secrets = m->secrets_found;
    p->score += PW_SCORE;
    refresh_score_text(p);
    data.amount = PW_SCORE;
    data.kind = POP_SECRET;
    spawn_popup(p, p->ww / 2.0f, p->wh / 2.0f + p->pitch - 60.0f, data);
}

void draw_pushwall_hint(sfRenderWindow *win, player_t *p, map_t *m)
{
    pwall_t *pw = pwall_near(p, m);
    sfText *t = NULL;
    sfFloatRect lb = {0};

    if (!pw)
        return;
    t = sfText_create();
    if (!t)
        return;
    sfText_setFont(t, p->hud_font);
    sfText_setCharacterSize(t, DOOR_HINT_SZ);
    sfText_setString(t, p->use_pad ? "CARRE : POUSSER" : "E : POUSSER");
    sfText_setFillColor(t, COL_POP_SECRET);
    lb = sfText_getLocalBounds(t);
    sfText_setPosition(t, (sfVector2f){(p->ww - lb.width) / 2.0f - lb.left,
        p->wh * DOOR_HINT_Y + 34.0f});
    sfRenderWindow_drawText(win, t, NULL);
    sfText_destroy(t);
}

int try_push_wall(player_t *p, map_t *m, sound_t *s)
{
    pwall_t *pw = pwall_near(p, m);

    if (!pw)
        return 0;
    push_dir(p, pw);
    if (!next_free(m, pw))
        return 0;
    pw->moving = sfTrue;
    pw->timer = PW_STEP_T;
    reward_secret(p, m, pw);
    step_pwall(m, pw);
    pwall_thunk(s, pw);
    return 1;
}

void update_pushwalls(player_t *p, map_t *m, sound_t *s)
{
    pwall_t *pw = NULL;

    for (int i = 0; i < m->pwall_count; i++) {
        pw = &m->pwalls[i];
        if (!pw->moving)
            continue;
        pw->timer -= p->dt;
        if (pw->timer > 0)
            continue;
        pw->timer = PW_STEP_T;
        step_pwall(m, pw);
        pwall_thunk(s, pw);
    }
}
