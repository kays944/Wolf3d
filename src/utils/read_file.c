/*
** EPITECH PROJECT, 2025
** robot_factory
** File description:
** read_file.c
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "utils.h"

int open_file(char *file)
{
    int fd = open(file, O_RDONLY);

    if (fd == OPEN_ERROR) {
        fprintf(stderr, "read: failure\n");
        return OPEN_ERROR;
    }
    return fd;
}

int verif_access_file(char *file)
{
    struct stat s;

    if (stat(file, &s) != 0) {
        fprintf(stderr, "read: failure\n");
        return STAT_ERROR;
    }
    return s.st_size;
}

char *read_file(char *file)
{
    int fd = 0;
    char *buffer = NULL;
    int size = verif_access_file(file);

    if (size == STAT_ERROR)
        return NULL;
    fd = open_file(file);
    if (fd == OPEN_ERROR)
        return NULL;
    buffer = malloc((size + 1) * sizeof(char));
    if (buffer == NULL)
        return NULL;
    if (read(fd, buffer, size) == READ_ERROR)
        return NULL;
    buffer[size] = '\0';
    if (close(fd) == CLOSE_ERROR)
        return NULL;
    return buffer;
}
