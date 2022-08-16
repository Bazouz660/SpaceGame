/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** load_ships.c
*/

#include "my.h"

static void write_value(FILE *f, int value)
{
    fwrite(my_int_to_str(value), 1, my_strlen(my_int_to_str(value)), f);
    fwrite("\n", 1, 1, f);
}

static void write_all_values(FILE *f, core_t *c)
{
    write_value(f, 1);
}

void save_in_file(core_t *c)
{
    FILE *f;
    char *save = malloc(sizeof(char) * 1000);
    int x = 0;
    char *filepath = malloc(sizeof(char) * 100);

    my_strcpy(filepath, "saves/");
    my_strcat(filepath, "save1");
    my_strcat(filepath, ".save");
    f = fopen(filepath, "w");
    fwrite("", 1, 0, f);
    fclose(f);
    f = fopen(filepath, "a");
    write_all_values(f, c);
    fclose(f);
}

static void set_vars(core_t *c, char **tmp)
{

}

char *read_file(char *location)
{
    struct stat st;
    char *buff;
    int i = 0;
    char *file = malloc(sizeof(char) * 300);
    int fd;

    file = my_strcpy(file, "ships/");
    file = my_strcat(file, location);
    fd = open(file, O_RDONLY);
    stat(file, &st);
    buff = malloc(sizeof(char) * st.st_size + 1);
    i = read(fd, buff, st.st_size);
    buff[i] = '\0';
    return (buff);
}

char *get_file_ext(const char *filename)
{
    int i = 0;
    int len =  my_strlen(filename);
    int last_dot = -1;
    char *ext = malloc(sizeof(char) * my_strlen(filename) + 1);

    for (;i < len; i++)
        if (filename[i] == '.')
            last_dot = i;
    if (last_dot == -1)
        return NULL;
    filename += last_dot;
    ext = my_strcpy(ext, filename);
    return ext;
}

char **find_files(void)
{
    find_files_t tf;
    int x = 0;

    tf.file_nb = 0;
    tf.d = opendir("ships/");
    while ((tf.dir = readdir(tf.d)) != NULL)
        if (my_strcmp(get_file_ext(tf.dir->d_name), ".shp") == 0)
            tf.file_nb++;
    tf.found_files = malloc(sizeof(char *) * tf.file_nb + 1);
    tf.d = opendir("ships/");
    for (int index = 0; (tf.dir = readdir(tf.d)) != NULL; x++)
        if (my_strcmp(get_file_ext(tf.dir->d_name), ".shp") == 0) {
            tf.file = open(tf.dir->d_name, O_RDONLY);
            tf.content = my_strdup(read_file(tf.dir->d_name));
            tf.content = my_strcat("\n", tf.content);
            tf.content = my_strcat(tf.dir->d_name, tf.content);
            tf.found_files[index] = my_strdup(tf.content);
            index++;
        }
    tf.found_files[x - 2] = NULL;
    if (tf.file_nb == 0) return NULL;
    return tf.found_files;
}

void print_found_files(void)
{
    char **found_files = find_files();

    for (int i = 0; found_files[i] != NULL; i++) {
        printf("file %d : %s\n\n", i, found_files[i]);
    }
}
