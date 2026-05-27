/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** parsing_map.c
*/

#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "wolf.h"
#include "utils.h"

static int row_has_player(char *row)
{
    for (int j = 0; row[j]; j++)
        if (row[j] == 'o')
            return EXIT_SUCCESS;
    return EXIT_FAIL;
}

static int has_player(char **map)
{
    for (int i = 0; map[i]; i++)
        if (row_has_player(map[i]) == EXIT_SUCCESS)
            return EXIT_SUCCESS;
    return EXIT_FAIL;
}

static void set_map_size(map_t *m)
{
    int len = 0;

    m->size_y = 0;
    m->size_x = 0;
    for (int i = 0; m->map[i]; i++) {
        m->size_y++;
        len = strlen(m->map[i]);
        if (len > m->size_x)
            m->size_x = len;
    }
}

void free_map(map_t *m)
{
    if (m->map)
        free_array(m->map);
    if (m->path)
        free(m->path);
    m->map = NULL;
    m->path = NULL;
}

static int validate_and_set(map_t *m, char *path)
{
    m->path = strdup(path);
    if (!m->path) {
        free_array(m->map);
        m->map = NULL;
        return EXIT_FAIL;
    }
    set_map_size(m);
    if (has_player(m->map) != EXIT_FAIL)
        return EXIT_SUCCESS;
    free(m->path);
    free_array(m->map);
    m->map = NULL;
    m->path = NULL;
    return EXIT_FAIL;
}

int parsing_map(map_t *m, char *path)
{
    char *buffer = read_file(path);

    if (!buffer)
        return EXIT_FAIL;
    m->map = str_split(buffer, '\n');
    free(buffer);
    if (!m->map)
        return EXIT_FAIL;
    return validate_and_set(m, path);
}
