/*
** EPITECH PROJECT, 2021
** tower defense
** File description:
** towerdefense.h
*/

#include "SFML/Graphics.h"
#include <SFML/Config.h>
#include <SFML/Audio.h>
#include <SFML/System.h>
#include <SFML/Window.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include "dirent.h"

#include <string.h>

#define True sfTrue
#define False sfFalse
#define false False
#define true True
#define PI 3.14159265359
#define bool int
#define transparant_red (sfColor){255, 0, 0, 100}

#define PLAYER_TEAM 0
#define SMOKE_MAX_VEL 20
#define MAX_SHIP_NB 10
#define Z_MAX 2
#define Z_MIN 0.5
#define SP_LEFT c->player->pos.x - ((c->render.size.x * 0.5) * Z_MAX)
#define SP_TOP c->player->pos.y - ((c->render.size.y * 0.5) * Z_MAX)
#define SP_WIDTH (c->render.size.x * Z_MAX)
#define SP_HEIGHT (c->render.size.y * Z_MAX)

typedef struct read_file_s {
    FILE *file;
    char *line;
    size_t len;
    int i;
    int lines;
} read_file_t;

typedef struct find_files_s {
    DIR *d;
    int file;
    struct dirent *dir;
    char *content;
    int file_nb;
    char **found_files;
    int index;
} find_files_t;

typedef struct particle_s {
    sfRectangleShape *part;
    sfVector2f vect;
    int index;
    int rot_dir;
    float angle;
    float fade_clock;
    float dtime;
    float scale;
    float scale_mul;
    float clock;
    float start_angle;
    float speed;
    float fade_time;
    float scale_time;
    float stop_time;
    double angular_vel;
    float base_alpha;
    struct particle_s *next;
    struct particle_s *prev;
} particle_t;

typedef struct smoke_params_s {
    float init_scale;
    float base_alpha;
    float fade_time;
    float scale_time;
    float stop_time;
    float speed;
    sfTexture *texture;
    sfColor color;
} smoke_params_t;

typedef struct smoke_emitter_s {
    sfVector2f pos;
    sfVector2f relative_pos;
    sfVector2f dir;
    particle_t *parts;
    sfSprite *sprite;
    float clock;
    float angle;
    struct smoke_emitter_s *next;
    struct smoke_emitter_s *prev;
} smoke_emitter_t;

typedef struct projectiles_s {
    sfSprite *sprite;
    sfVector2f vect;
    int team;
    float dmg;
    struct projectiles_s *next;
    struct projectiles_s *prev;
} projectiles_t;

typedef struct gun_s {
    float recoil;
    float cooldown;
} gun_t;

typedef struct directions_s {
    sfBool forward;
    sfBool backward;
    sfBool right;
    sfBool left;
} directions;

typedef struct shield_s {
    float max_nrj;
    float current_nrj;
    float nrj_reg_speed;
    bool nrj_can_reg;
    float nrj_reg_cooldown_time;
    float nrj_reg_cooldown_clock;
    float nrj_reg_ammount;
    float nrj_clock;
    sfRectangleShape *sprite;
} shield_t;

typedef struct model_s {
    sfSprite *sprite;
    smoke_emitter_t *exhausts;
    gun_t gun;
} model_t;

typedef struct ship_s {
    model_t *model;
    shield_t shield;
    directions direction;
    float speed;
    float integrity;
    float max_integrity;
    float shoot_clock;
    float turn_speed;
    double angle;
    double acceleration;
    double acceleration_max;
    int index;
    int team;
    int type;
    bool triggered;
    bool player;
    sfVector2f vect;
    sfVector2f pos;
    sfSound *shoot;
    struct ship_s *next;
    struct ship_s *prev;
} ship_t;

typedef struct explosion_s {
    sfVector2f pos;
    particle_t *parts;
    sfSound *sound;
    float clock;
    float size;
    int index;
    struct explosion_s *next;
    struct explosion_s *prev;
} explosion_t;

typedef struct player_s {
    sfSprite *sprite;
    smoke_emitter_t *exhausts;
    shield_t shield;
    gun_t gun;
    directions direction;
    float integrity;
    float max_integrity;
    float speed;
    float shoot_clock;
    double angle;
    double acceleration;
    sfVector2f vect;
    sfVector2f pos;
} player_t;

typedef struct window_s {
    sfRenderWindow *window;
    sfView *view;
    sfVector2f view_pos;
    sfSprite *cursor;
    sfText *fps_hint;
    float fps_clock;
    float zoom_factor;
    sfVector2u size;
    sfFont *font;
} render_t;

typedef struct clock_s {
    sfClock *clock;
    sfTime time;
    sfTime prev_time;
    float seconds;
} clock_st;

typedef struct textures_s {
    sfTexture *smoke;
    sfTexture *player;
    sfTexture *explosion;
    sfTexture *bolt;
    sfTexture *shield;
    sfTexture *ships[8];
    sfTexture *debris[6];
    sfTexture *asteroid;
    sfTexture *crosshair;
    sfTexture *health_bar_in;
    sfTexture *shield_bar_out;
    sfTexture *shield_bar_in;
} textures_t;

typedef struct stars_s {
    sfCircleShape *star;
    sfVector2f pos;
    struct stars_s *next;
    struct stars_s *prev;
} stars_t;

typedef struct mouse_s {
    sfVector2f pos;
    sfVector2f last_pos;
    sfVector2f diff;
} mouse_t;

typedef struct sounds_s {
    sfSound *thruster;
    sfSound *blaster;
    sfSound *impact;
    sfSound *shield_hit;
    sfSound *shield_down;
    sfSound *shield_boot;
    sfSoundBuffer *explosion;
} sounds_t;

typedef struct entities_s {
    sfVector2f spawn_pos[9];
    projectiles_t *bolts;
    explosion_t *explosions;
    ship_t *ships;
    int ships_nb;
} entities_t;

typedef struct shader_s {
    sfShader *boxblur;
    sfShader *luminescence;
    sfShader *assemble;
    sfShader *multiply;
    sfRenderStates render_states;
    sfRectangleShape *postproc_rect;
    sfTexture *postproc_txtr;
    sfTexture *f_buffer1;
    sfTexture *f_buffer2;
    sfTexture *f_buffer3;
} shader_t;

typedef struct visualizer_s {
    short *samples;
    sfSound *song;
    sfSoundBuffer *sbuffer;
} visualizer_t;

typedef struct hud_s {
    sfSprite *shield_bar_out;
    sfRectangleShape *shield_bar_in;
    sfRectangleShape *health_bar_in;
    sfText *shield_nrj_txt;
} hud_t;

typedef struct core_s {
    render_t render;
    clock_st clock;
    ship_t *player;
    sfEvent event;
    entities_t entities;
    textures_t textures;
    sounds_t sounds;
    stars_t *skybox;
    mouse_t mouse;
    shader_t shader;
    hud_t hud;
} core_t;

// Utils
sfRenderWindow *create_window(char const *title);
sfSprite *new_sprite(char *texture_name, float x, float y);
sfSound *set_up_new_sound(sfSound *new_sound, const char *buffer_location);
void init_animated_sprite(sfSprite *sprite, int columns, int rows);
sfSprite *setSprite_pos(sfSprite *sprite, float posX, float posY);
sfVector2f get_sprite_center(sfSprite *sprite);
float random_float(float scale);
int random_number(int min_num, int max_num);
sfVertex add_vertex_circular(core_t *c, float *radius,
float i, sfVector2f center);
int positive(int nb);
sfVector2f get_rect_center(sfRectangleShape *rect);
sfFloatRect init_frect(float left, float top, float width, float height);
int rand_range(int lower, int upper);
sfVector2f init_vector2f(float x, float y);
sfIntRect init_irect(int left, int top, int width, int height);
char *my_int_to_str(int nb);
double get_angle(sfVector2f origin, sfVector2f direction);
double rad_to_deg(double angle);
double deg_to_rad(double angle);
sfBool circle_intersect(sfVector2f center, float radius, sfVector2f point);
sfFloatRect view_bounds(sfView *view);
sfVector2f get_mouse_pos_view(core_t *c);
sfBool get_mouse_intersect_view(core_t *c, sfFloatRect to_check);
double absolute(double value);
sfBool circles_intersect(sfVector2f center, float radius,
sfVector2f center2, float radius2);
sfVector2f sprite_gethead(sfSprite *sprite, sfVector2f offset);
void modulate_sound(sfSound *sound);
float get_sound_mid(visualizer_t *vz);
int my_strlen(char const *str);
char *my_strcat(char *dest, char const *src);
char *my_strcpy(char *dest, char const *src);
char *my_strdup(char const *src);
int my_strcmp(char const *s1, char const *s2);
char **strwar(const char *str, const char *separator);

// Linked lists
void add_part(core_t *c, particle_t **head, smoke_emitter_t *emiter);
void del_part(particle_t **head, particle_t *del_node);

// Smoke
void handle_smoke(core_t *c, smoke_emitter_t *smoke, float mag);
void new_explosion(core_t *c, explosion_t **head, sfVector2f pos, float size);
void del_explosion(explosion_t **head, explosion_t *del_node);
void render_smoke(core_t *c, particle_t **head, particle_t *node);
void draw_all_explosions(core_t *c);
void draw_smoke(core_t *c, particle_t *node);

// Vectors
sfVector2f vect_add(sfVector2f v1, sfVector2f v2);
sfVector2f vect_sub(sfVector2f v1, sfVector2f v2);
float vect_mag(sfVector2f v);
sfVector2f vect_fmult(sfVector2f v, float n);
sfVector2f vect_unit(sfVector2f v);
sfVector2f vect_norm(sfVector2f v);
float vect_dot(sfVector2f v1, sfVector2f v2);
float vect_cross(sfVector2f v1, sfVector2f v2);
float dist_from(sfVector2f p1, sfVector2f p2);
sfVector2f closest_point_seg(sfVector2f a, sfVector2f b, sfVector2f p);
sfVector2f vect_vmult(sfVector2f v, sfVector2f v2);
sfVector2f vect2u_vect2f(sfVector2u v);
sfVector2f clamp_vect2f(sfVector2f v, float maxm);

// Fps
float get_fps(sfClock *clock, sfTime *prev_time);
float time_as_sec(sfTime time);

// Exhausts
void add_exhaust(core_t *c, smoke_emitter_t **head, sfVector2f relative_pos);
void del_exhaust(particle_t **head, particle_t *del_node);
void update_exhaust(core_t *c, ship_t *ship);

// Skybox
void del_star(stars_t **head, stars_t *del_node);
void add_star(core_t *c, stars_t **head, sfBool away);

// Guns
void del_bolt(projectiles_t **head, projectiles_t *del_node);
void add_bolt(core_t *c, projectiles_t **head, ship_t *src);

// Ships
void del_ship(ship_t **head, ship_t *del_node);
void add_ship(core_t *c, ship_t **head, sfVector2f pos, int team, int type, bool player);
void randomise_spawns(core_t *c);
void orbit_target(core_t *c, ship_t *ship, sfVector2f target);
void avoid_allies(core_t *c, ship_t *ship);
float find_target(core_t *c, ship_t *ship, sfVector2f target);
void engage_target(core_t *c, ship_t *ship);

// Inits
void init_visualizer(visualizer_t vi);
void init_game(core_t *c);
void init_shaders(core_t *c);
void init_hud(core_t *c);
void init_textures(core_t *c);
void init_sky(core_t *c);
void init_sounds(core_t *c);
void init_view(core_t *c);
void init_player(core_t *c);
void init_fps_text(core_t *c);

// Loading
void print_found_files(void);
