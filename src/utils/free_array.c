/*
** EPITECH PROJECT, 2025
** mysh1
** File description:
** free_array.c
*/

#include <stdlib.h>

void free_array(char **arr)
{
    for (int i = 0; arr[i]; ++i)
        free(arr[i]);
    free(arr);
}
