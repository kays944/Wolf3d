/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** parsing_map.c
*/

#include "macros.h"
#include "wolf.h"
#include "utils.h"

char **parsing_map(char *path)
{
    char *buffer = read_file(path);
    char **map = NULL;

    if (!buffer)
        return NULL;
    map = str_split(buffer, '\n');
    if (!map)
        return NULL;
    return map;
}
