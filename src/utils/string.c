/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** string.c
*/

#include "my.h"

char *my_strcpy(char *dest, char const *src)
{
    int i = 0;

    for (i = 0; src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return (dest);
}

char *my_strdup(char const *src)
{
    char *str = malloc(sizeof(char) * (my_strlen(src) + 1));

    for (int i = 0; src[i] != '\0'; i++)
        str[i] = src[i];
    str[my_strlen(str)] = '\0';
    return (str);
}

int my_strcmp(char const *s1, char const *s2)
{
    int i = 0;

    while (s1[i] == s2[i]) {
        if (s1[i] == '\0' && s2[i] == '\0')
            return (0);
        i++;
    }
    if (s1[i] == '\0' || (s1[i] < s2[i] && s2[i] != '\0'))
        return (1);
    return (-1);
}

int my_strncmp(char const *s1, char const *s2, int n)
{
    int i = 0;

    while (s1[i] == s2[i]) {
        if ((s1[i] == '\0' && s2[i] == '\0') || i == n - 1)
            return (0);
        i++;
    }
    if (s1[i] == '\0' || (s1[i] < s2[i] && s2[i] != '\0'))
        return (1);
    return (-1);
}

char *rm_str_char(char **str, char *to_remove)
{
    char *tmp = *str;
    char *res = malloc(sizeof(char) * (my_strlen(tmp)));
    int x = 0;
    int found = 0;

    for (int i = 0; tmp[i] != '\0'; i++) {
        found = 0;
        for (int z = 0; to_remove[z] != '\0'; z++)
            if (tmp[i] == to_remove[z]) {
                found = 1;
                break;
            }
        if (found == 0) {
            res[x] = tmp[i];
            x++;
        }
    }
    res[x] = '\0';
    free(tmp);
    return res;
}
