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

ship_t *analyse_file(char **file, int index)
{
    ship_t *ship = malloc(sizeof(ship_t));
    shield_t shield;
    sfVector2f *s_ems = NULL;
    char *texture_name = NULL;
    float cooldown = NAN;
    float health = NAN;
    float recoil = NAN;
    float acceleration = NAN;
    float speed = NAN;
    float turn_speed = NAN;

    ship->name = NULL;
    shield.max_nrj = NAN;
    shield.nrj_reg_ammount = NAN;
    shield.nrj_reg_cooldown_time = NAN;
    shield.nrj_reg_speed = NAN;
    for (int i = 0; file[i] != NULL; i++) {
        file[i] = rm_str_char(&file[i], " \t");
        if (file[i][0] == COMMENT_CHAR)
            continue;
        if (i == 0)
            ship->name = my_strdup(file[i]);
        if (my_strncmp("cooldown=", file[i], 9) == 0)
            cooldown = atof(file[i] + 9);
        if (my_strncmp("speed=", file[i], 6) == 0)
            speed = atof(file[i] + 6);
        if (my_strncmp("health=", file[i], 7) == 0)
            health = atof(file[i] + 7);
        if (my_strncmp("acceleration=", file[i], 13) == 0)
            acceleration = atof(file[i] + 13);
        if (my_strncmp("recoil=", file[i], 7) == 0)
            recoil = atof(file[i] + 7);
        if (my_strncmp("turn_speed=", file[i], 11) == 0)
            turn_speed = atof(file[i] + 11);
        if (my_strncmp("max_nrj=", file[i], 8) == 0)
            shield.max_nrj = atof(file[i] + 8);
        if (my_strncmp("nrj_reg_ammount=", file[i], 16) == 0)
            shield.nrj_reg_ammount = atof(file[i] + 16);
        if (my_strncmp("nrj_reg_cooldown_time=", file[i], 22) == 0)
            shield.nrj_reg_cooldown_time = atof(file[i] + 22);
        if (my_strncmp("nrj_reg_speed=", file[i], 14) == 0)
            shield.nrj_reg_speed = atof(file[i] + 14);
        if (my_strncmp("texture_name=", file[i], 13) == 0)
            texture_name = file[i] + 13;
    }

    if (cooldown != cooldown || health != health || speed != speed || recoil != recoil
        || acceleration != acceleration || turn_speed != turn_speed || shield.max_nrj != shield.max_nrj
        || shield.nrj_reg_ammount != shield.nrj_reg_ammount || shield.nrj_reg_cooldown_time != shield.nrj_reg_cooldown_time
        || shield.nrj_reg_speed != shield.nrj_reg_speed) {
        ship->type = -1;
        free(ship);
        return NULL;
    }

    printf("\nship name = %s\n", ship->name);
    printf("cooldown = %.2f\n", cooldown);
    printf("speed = %.2f\n", speed);
    printf("health = %.2f\n", health);
    printf("acceleration = %.2f\n", acceleration);
    printf("recoil = %.2f\n", recoil);
    printf("type = %d\n", index);
    printf("turn_speed = %.2f\n", turn_speed);
    printf("max_nrj = %.2f\n", shield.max_nrj);
    printf("nrj_reg_ammount = %.2f\n", shield.nrj_reg_ammount);
    printf("nrj_reg_cooldown_time = %.2f\n", shield.nrj_reg_cooldown_time);
    printf("nrj_reg_speed = %.2f\n", shield.nrj_reg_speed);
    printf("texture_name = %s\n", texture_name);
    return ship;
}

void get_valid_shp(void)
{
    char **found_files = find_files();
    int len = get_arr_len((void const **)found_files);
    printf("found %d files\n", len);
    ship_t **shp_list = malloc(sizeof(ship_t *) * len);
    ship_t *tmp = NULL;
    int x = 0;

    for (int i = 0; found_files[i] != NULL; i++) {
        tmp = analyse_file(strwar(found_files[i], "\n"), i);
        if (tmp != NULL) {
            shp_list[x] = tmp;
            x++;
        }
    }
    if (x <= 1)
        printf("\n%d file was validated\n\n", x);
    else
        printf("\n%d files were validated\n\n", x);
}
