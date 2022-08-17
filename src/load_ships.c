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
    close(fd);
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

int get_nb_files(const char *path)
{
    find_files_t tf;
    DIR *d;
    struct dirent *dir;
    int x = 0;
    int nb = 0;

    if ((d = opendir("ships/")) == NULL)
        return -1;
    while ((dir = readdir(d)) != NULL)
        nb++;
    closedir(d);
    return nb - 2;
}

char **find_files(void)
{
    find_files_t tf;
    int index = 0;

    tf.file_nb = get_nb_files("ships/");
    tf.content = NULL;
    if (tf.file_nb == -1)
        return NULL;
    tf.found_files = malloc(sizeof(char *) * (tf.file_nb + 1));
    if ((tf.d = opendir("ships/")) == NULL)
        return NULL;
    while ((tf.dir = readdir(tf.d)) != NULL) {
        if (my_strcmp(get_file_ext(tf.dir->d_name), ".shp") == 0) {
            tf.file = open(tf.dir->d_name, O_RDONLY);
            tf.content = my_strdup(read_file(tf.dir->d_name));
            tf.content = my_strcat("\n", tf.content);
            tf.content = my_strcat(tf.dir->d_name, tf.content);
            tf.found_files[index] = my_strdup(tf.content);
            index++;
        }
    }
    tf.found_files[index] = NULL;
    if (tf.file_nb == 0)
        return NULL;
    return tf.found_files;
}

void print_shp_info(ship_t *ship)
{
    printf("\nship name = %s\n", ship->name);
    printf("cooldown = %.2f\n", ship->model->gun.cooldown);
    printf("speed = %.2f\n", ship->speed);
    printf("health = %.2f\n", ship->integrity);
    printf("acceleration = %.2f\n", ship->acceleration);
    printf("recoil = %.2f\n", ship->model->gun.recoil);
    printf("type = %d\n", ship->type);
    printf("turn_speed = %.2f\n", ship->turn_speed);
    printf("max_nrj = %.2f\n", ship->shield.max_nrj);
    printf("nrj_reg_ammount = %.2f\n", ship->shield.nrj_reg_ammount);
    printf("nrj_reg_cooldown_time = %.2f\n", ship->shield.nrj_reg_cooldown_time);
    printf("nrj_reg_speed = %.2f\n", ship->shield.nrj_reg_speed);
    printf("texture_name = %s\n", ship->texture_name);
}

ship_t *analyse_file(char **file, int index)
{
    ship_t *ship = malloc(sizeof(ship_t));
    sfVector2f *s_ems = NULL;

    ship->model = malloc(sizeof(model_t));
    ship->type = index;
    ship->model->gun.recoil = NAN;
    ship->integrity = NAN;
    ship->acceleration = NAN;
    ship->speed = NAN;
    ship->turn_speed = NAN;
    ship->model->gun.cooldown = NAN;
    ship->texture_name = NULL;
    ship->name = NULL;
    ship->shield.max_nrj = NAN;
    ship->shield.nrj_reg_ammount = NAN;
    ship->shield.nrj_reg_cooldown_time = NAN;
    ship->shield.nrj_reg_speed = NAN;

    for (int i = 0; file[i] != NULL; i++) {
        file[i] = rm_str_char(&file[i], " \t");
        if (file[i][0] == COMMENT_CHAR)
            continue;
        if (i == 0)
            ship->name = my_strdup(file[i]);
        if (my_strncmp("cooldown=", file[i], 9) == 0) {
            ship->model->gun.cooldown = atof(file[i] + 9);
        }
        if (my_strncmp("speed=", file[i], 6) == 0)
            ship->speed = atof(file[i] + 6);
        if (my_strncmp("health=", file[i], 7) == 0)
            ship->integrity = atof(file[i] + 7);
        if (my_strncmp("acceleration=", file[i], 13) == 0)
            ship->acceleration = atof(file[i] + 13);
        if (my_strncmp("recoil=", file[i], 7) == 0)
            ship->model->gun.recoil = atof(file[i] + 7);
        if (my_strncmp("turn_speed=", file[i], 11) == 0)
            ship->turn_speed = atof(file[i] + 11);
        if (my_strncmp("max_nrj=", file[i], 8) == 0)
            ship->shield.max_nrj = atof(file[i] + 8);
        if (my_strncmp("nrj_reg_ammount=", file[i], 16) == 0)
            ship->shield.nrj_reg_ammount = atof(file[i] + 16);
        if (my_strncmp("nrj_reg_cooldown_time=", file[i], 22) == 0)
            ship->shield.nrj_reg_cooldown_time = atof(file[i] + 22);
        if (my_strncmp("nrj_reg_speed=", file[i], 14) == 0)
            ship->shield.nrj_reg_speed = atof(file[i] + 14);
        if (my_strncmp("texture_name=", file[i], 13) == 0) {
            ship->texture_name = my_strdup(file[i] + 13);
        }
    }
    if (ship->model->gun.cooldown != ship->model->gun.cooldown ||
        ship->integrity != ship->integrity || ship->speed != ship->speed ||
        ship->model->gun.recoil != ship->model->gun.recoil
        || ship->acceleration != ship->acceleration || ship->turn_speed != \
        ship->turn_speed || ship->shield.max_nrj != ship->shield.max_nrj
        || ship->shield.nrj_reg_ammount != ship->shield.nrj_reg_ammount ||
        ship->shield.nrj_reg_cooldown_time != \
        ship->shield.nrj_reg_cooldown_time
        || ship->shield.nrj_reg_speed != ship->shield.nrj_reg_speed || \
        ship->texture_name == NULL) {
        ship->type = -1;
        return NULL;
    }
    return ship;
}

void copy_content(ship_t **shp_list, ship_t *shp, int *index)
{
    if (shp != NULL) {
        shp_list[*index] = shp;
        *index += 1;
    }
}

ship_t **get_valid_shp(core_t *c)
{
    char **found_files = find_files();
    if (found_files == NULL)
        return NULL;
    int len = get_arr_len((void const **)found_files);
    printf("\n\n======= found %d files =======\n", len);
    ship_t **shp_list = malloc(sizeof(ship_t *) * (len + 1));
    ship_t *tmp = NULL;
    int x = 0;

    for (int i = 0; found_files[i] != NULL; i++) {
        copy_content(shp_list, analyse_file(strwar(found_files[i], "\n"), i), &x);
    }
    shp_list[x] = NULL;
    for (int i = 0; shp_list[i] != NULL; i++) {
        printf("\n");
        print_shp_info(shp_list[i]);
        printf("\n");
    }
    if (x <= 1)
        printf("======= %d file was validated on %d =======\n\n", x, len);
    else
        printf("======= %d files were validated on %d =======\n\n", x, len);
    return shp_list;
}
