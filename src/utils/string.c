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
