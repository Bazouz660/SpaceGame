/*
** EPITECH PROJECT, 2022
** smoke test
** File description:
** explosion.c
*/

#include "my.h"

void add_smoke_exp(core_t *c, particle_t **head,
sfVector2f pos, sfVector2f center, smoke_params_t params)
{
	static int index = 0;
	particle_t *new_part;
    sfColor color = params.color;

	new_part = malloc(sizeof(particle_t));
    new_part->part = sfRectangleShape_create();
	new_part->index = index;
    color.a = params.base_alpha;
    new_part->base_alpha = color.a;
    sfRectangleShape_setFillColor(new_part->part, color);
    sfRectangleShape_setSize(new_part->part, (sfVector2f){20, 20});
    sfRectangleShape_setPosition(new_part->part, pos);
    new_part->angle = random_float(180);
	if (random_number(-100, 100) < 0)
        new_part->rot_dir = -1;
	else
	    new_part->rot_dir = 1;
    new_part->angular_vel = 0.1;
    new_part->scale = params.init_scale;
    new_part->fade_clock = 0;
	new_part->scale_mul = 0.1;
    new_part->scale_time = params.scale_time;
    new_part->stop_time = params.stop_time;
    new_part->fade_time = params.fade_time;
    new_part->speed = params.speed;
	new_part->dtime = 0;
    new_part->start_angle = get_angle(center, pos);
    sfRectangleShape_setOrigin(new_part->part, get_rect_center(new_part->part));
    sfRectangleShape_setRotation(new_part->part, new_part->angle);
    sfRectangleShape_setTexture(new_part->part, params.texture, sfTrue);
    new_part->clock = c->clock.seconds;
    new_part->vect = (sfVector2f){cos(deg_to_rad(get_angle(center, pos))) * 15,
	sin(deg_to_rad(get_angle(center, pos))) * 15};
	new_part->next = (*head);
	new_part->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = new_part;
	(*head) = new_part;
	index++;
}

sfVector2f *divide_area(core_t *c, sfVector2f center, int chunks)
{
    double degrees = 0;
    sfVector2f *coords = malloc(sizeof(sfVector2f) * chunks);
    double radius = 20;

    for (int i = 0; i < chunks; i++) {
        degrees = i * ((double)360 / chunks);
        coords[i].x = center.x + radius * cos(deg_to_rad(degrees));
        coords[i].y = center.y + radius * sin(deg_to_rad(degrees));
    }
    return coords;
}

void new_explosion(core_t *c, explosion_t **head, sfVector2f pos, float size)
{
	static int index = 0;
	explosion_t *nnode;
    sfVector2f *area = divide_area(c, pos, 20);
    smoke_params_t params;

	nnode = malloc(sizeof(explosion_t));
	nnode->index = index;
    nnode->clock = 0;
    nnode->pos = pos;
    nnode->size = size;
    nnode->parts = NULL;
    nnode->sound = sfSound_create();
    sfSound_setBuffer(nnode->sound, c->sounds.explosion);
    modulate_sound(nnode->sound);
    sfSound_play(nnode->sound);
    params.base_alpha = 120;
    params.init_scale = 1.5;
    params.scale_time = 1.2;
    params.stop_time = 0.8;
    params.fade_time = 1.2;
    params.color = sfWhite;
    params.color.a = params.base_alpha;
    params.texture = c->textures.smoke;
    for (int i = 0; i < 20; i++) {
        params.speed = (float)random_number(15, 16) / 10;
        add_smoke_exp(c, &nnode->parts,
        area[i], pos, params);
    }
    params.stop_time = 1;
    params.fade_time = 2;
    params.scale_time = 1.2;
    params.base_alpha = 80;
    params.init_scale = 8;
    params.texture = c->textures.explosion;
    for (int i = 0; i < 30; i++) {
        params.speed = (float)random_number(15, 30) / 10;
        add_smoke_exp(c, &nnode->parts,
        (sfVector2f){random_number(pos.x - 20, pos.x + 20),
        random_number(pos.y - 20, pos.y + 20)}, pos, params);
    }
    params.texture = c->textures.smoke;
    for (int i = 0; i < 30; i++) {
        params.speed = (float)random_number(15, 30) / 10;
        add_smoke_exp(c, &nnode->parts,
        (sfVector2f){random_number(pos.x - 20, pos.x + 20),
        random_number(pos.y - 20, pos.y + 20)}, pos, params);
    }

    params.stop_time = 6;
    params.fade_time = 6;
    params.scale_time = 0.0001;
    params.base_alpha = 255;
    params.init_scale = 4.5;
    for (int i = 0; i < random_number(3, 6); i++) {
        params.texture = c->textures.debris[random_number(0, 5)];
        params.speed = (float)random_number(10, 25) / 10;
        add_smoke_exp(c, &nnode->parts,
        (sfVector2f){random_number(pos.x - 20, pos.x + 20),
        random_number(pos.y - 20, pos.y + 20)}, pos, params);
    }

    params.texture = c->textures.explosion;
    params.base_alpha = 255;
    params.scale_time = 0.1;
    params.stop_time = 3;
    params.fade_time = 0.8;
    params.color = (sfColor){255, 165, 0, 255};
    for (int i = 0; i < random_number(15, 20); i++) {
        params.speed = (float)random_number(100, 200) / 10;
        params.init_scale = (float)random_number(2, 8) / 10;
        add_smoke_exp(c, &nnode->parts,
        (sfVector2f){random_number(pos.x - 20, pos.x + 20),
        random_number(pos.y - 20, pos.y + 20)}, pos, params);
    }

	nnode->next = (*head);
	nnode->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = nnode;
	(*head) = nnode;
	index++;
    free(area);
}

void del_explosion(explosion_t **head, explosion_t *del_node)
{
	if (*head == NULL || del_node == NULL)
		return ;
	if (*head == del_node)
		*head = del_node->next;
	if (del_node->next != NULL)
		del_node->next->prev = del_node->prev;
	if (del_node->prev != NULL)
		del_node->prev->next = del_node->next;
    sfSound_destroy(del_node->sound);
	free(del_node);
}