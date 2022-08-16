/*
** EPITECH PROJECT, 2021
** smoke test
** File description:
** smoke.c
*/

#include "my.h"

void add_part(core_t *c, particle_t **head, smoke_emitter_t *emiter)
{
	static int index = 0;
	particle_t *new_part;
	sfVector2f pos = emiter->pos;

	new_part = malloc(sizeof(particle_t));
    new_part->part = sfRectangleShape_create();
	new_part->index = index;
	new_part->base_alpha = 50;
    sfRectangleShape_setFillColor(new_part->part, (sfColor){150, 150, 150,
	new_part->base_alpha});
    sfRectangleShape_setSize(new_part->part, (sfVector2f){20, 20});
    sfRectangleShape_setPosition(new_part->part, pos);
    new_part->angle = random_float(180);
	if (random_number(-100, 100) < 0)
        new_part->rot_dir = -1;
	else
	    new_part->rot_dir = 1;
    new_part->angular_vel = 0.1;
    new_part->scale = 1;
    new_part->fade_clock = 0;
	new_part->dtime = 0;
	new_part->clock = c->clock.seconds;
	new_part->scale_mul = 0.1;
    new_part->scale_time = 0.7;
    new_part->stop_time = 1;
    new_part->fade_time = 0.5;
	new_part->speed = 1;
	new_part->start_angle = emiter->angle;
    sfRectangleShape_setOrigin(new_part->part, get_rect_center(new_part->part));
    sfRectangleShape_setRotation(new_part->part, new_part->angle);
    sfRectangleShape_setTexture(new_part->part, c->textures.smoke, sfTrue);
    new_part->vect = (sfVector2f){0, 0};
	new_part->next = (*head);
	new_part->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = new_part;
	(*head) = new_part;
	index++;
}

void del_part(particle_t **head, particle_t *del_node)
{
	if (*head == NULL || del_node == NULL)
		return ;
	if (*head == del_node)
		*head = del_node->next;
	if (del_node->next != NULL)
		del_node->next->prev = del_node->prev;
	if (del_node->prev != NULL)
		del_node->prev->next = del_node->next;
	free(del_node);
}


void update_color(core_t *c, sfColor *color, particle_t *node)
{
    float prog = node->dtime / node->fade_time;

    if (prog > 1)
        prog = 1;
    color->a = node->base_alpha * (1 - prog);
    sfRectangleShape_setFillColor(node->part, *color);
}

void destroy_old(sfColor color, particle_t **head, particle_t *node)
{
    if (color.a <= 0) {
       sfRectangleShape_destroy(node->part);
       del_part(head, node);
    }
}

void translate_part(particle_t *node)
{
    sfVector2f upward_force = {0, 0};
    float prog = node->dtime / node->stop_time;

    if (prog > 1)
        prog = 1;
    node->vect.x = (2 - (log10(prog + 0.1) + 1));
    node->vect.y = (2 - (log10(prog + 0.1) + 1));
    if (node->vect.x < 0)
        node->vect.x = 0;
    if (node->vect.y < 0)
        node->vect.y = 0;
    node->vect.x *= cos(deg_to_rad(node->start_angle)) * -1;
    node->vect.y *= sin(deg_to_rad(node->start_angle)) * -1;
    node->vect.x *= node->speed;
    node->vect.y *= node->speed;
    sfRectangleShape_move(node->part, node->vect);
    sfRectangleShape_move(node->part, upward_force);
}

void scale_part(particle_t *node)
{
    float prog = node->dtime / node->scale_time;

    if (node->scale_time == 0)
        return;
    if (prog > 1)
        prog = 1;
    node->scale_mul = (1 - (log10(prog + 0.1) + 1)) * 0.15;
    node->scale += node->scale_mul;
    sfRectangleShape_setScale(node->part, (sfVector2f){node->scale,
    node->scale});
}

void rotate_part(particle_t *node)
{
    float prog = node->dtime / 10;

    if (prog > 1)
        prog = 1;
    node->angular_vel = (1 - (log10(prog + 0.1) + 1)) * 1;
    node->angular_vel *= node->rot_dir;
    node->angle += node->angular_vel;
    sfRectangleShape_setRotation(node->part, node->angle);
}

void move_part(particle_t *node)
{
    translate_part(node);
    scale_part(node);
    rotate_part(node);
}

void render_smoke(core_t *c, particle_t **head, particle_t *node)
{
    sfColor color;
    sfVector2f scale;
    float upward_force = 1;

    while (node != NULL) {
        if (c->clock.seconds > node->clock) {
            node->dtime += c->clock.seconds - node->clock;
            node->clock = c->clock.seconds;
        }
        color = sfRectangleShape_getFillColor(node->part);
        update_color(c, &color, node);
        move_part(node);
        destroy_old(color, head, node);
        node = node->next;
    }
}

void draw_smoke(core_t *c, particle_t *node)
{
    while (node != NULL) {
        sfRenderWindow_drawRectangleShape(c->render.window,
        node->part, NULL);
        node = node->next;
    }
}

void handle_smoke(core_t *c, smoke_emitter_t *smoke, float mag)
{
    if (mag > 1
        && c->clock.seconds > smoke->clock + 0.01) {
        smoke->clock = c->clock.seconds;
        for (int i = 0; i < 3; i++)
            add_part(c, &smoke->parts, smoke);
    }
    render_smoke(c, &smoke->parts, smoke->parts);
}