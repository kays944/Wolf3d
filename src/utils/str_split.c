/*
** EPITECH PROJECT, 2025
** str to word array
** File description:
** str to word array
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int is_blank(char c)
{
    return c == ' ' || c == '\t';
}

static int next_token(char *str, char sep, int *i, int range[2])
{
    while (str[*i]) {
        while (str[*i] == sep)
            (*i)++;
        if (!str[*i])
            return 0;
        range[0] = *i;
        while (str[*i] && str[*i] != sep)
            (*i)++;
        range[1] = *i;
        while (range[0] < range[1] && is_blank(str[range[0]]))
            range[0]++;
        while (range[1] > range[0] && is_blank(str[range[1] - 1]))
            range[1]--;
        if (range[1] > range[0])
            return 1;
    }
    return 0;
}

static int str_count_tokens(char *str, char separator)
{
    int i = 0;
    int range[2];
    int count = 0;

    while (next_token(str, separator, &i, range))
        count++;
    return count;
}

static void fill_result(char **result, char *str, char separator)
{
    int i = 0;
    int range[2];
    int token_idx = 0;

    while (next_token(str, separator, &i, range)) {
        result[token_idx] = strndup(str + range[0], range[1] - range[0]);
        if (!result[token_idx])
            return;
        token_idx++;
    }
    result[token_idx] = NULL;
}

char **str_split(char *str, char separator)
{
    int count = 0;
    char **result;

    if (str == NULL)
        return NULL;
    for (int i = 0; str[i]; i++)
        if (str[i] == '\t')
            str[i] = ' ';
    count = str_count_tokens(str, separator);
    if (count == 0)
        return NULL;
    result = malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;
    fill_result(result, str, separator);
    return result;
}
