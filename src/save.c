/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** save.c
*/

#include <fcntl.h>
#include <unistd.h>
#include "macros.h"
#include "proto.h"

static char get_tile(char **map, int i, int j, player_t *player)
{
    int ptx = (int)(player->x / TILE_SIZE);
    int pty = (int)(player->y / TILE_SIZE);

    if (i == pty && j == ptx)
        return 'o';
    if (map[i][j] == 'o')
        return ' ';
    return map[i][j];
}

static void write_row(int fd, char **map, int i, player_t *player)
{
    int j = 0;
    char c = 0;

    for (j = 0; map[i][j]; j++) {
        c = get_tile(map, i, j, player);
        write(fd, &c, 1);
    }
    c = '\n';
    write(fd, &c, 1);
}

void saving(char **map, player_t *player)
{
    int fd = open(MAP_SAVE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        write(2, "Warning: could not create map_save.wolf\n", 40);
        return;
    }
    for (int i = 0; map[i]; i++)
        write_row(fd, map, i, player);
    close(fd);
}
