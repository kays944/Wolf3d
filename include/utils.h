/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** utils.h
*/

#ifndef UTILS_H_
    #define UTILS_H_
    #define OPEN_ERROR -1
    #define STAT_ERROR -1
    #define CLOSE_ERROR -1
    #define READ_ERROR -1

void free_array(char **arr);
char *read_file(char *file);
char **str_split(char *str, char separator);

#endif
