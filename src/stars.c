/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** stars.c
*/

#include "my.h"

void add_star(core_t *c, stars_t **head, sfBool away)
{
	static int index = 0;
	stars_t *new_part;
	float x_ratio = c->player->vect.x / c->player->vect.y;
	float y_ratio = c->player->vect.y / c->player->vect.x;

	new_part = malloc(sizeof(stars_t));
    new_part->star = sfCircleShape_create();
	if (away == True)
	    sfCircleShape_setPosition(new_part->star,
		c->entities.spawn_pos[random_number(0, 7)]);
	else
	    sfCircleShape_setPosition(new_part->star,
		c->entities.spawn_pos[random_number(0, 8)]);
    sfCircleShape_setFillColor(new_part->star, sfWhite);
    sfCircleShape_setRadius(new_part->star, ((float)random_number(10, 300) / 10));
	sfCircleShape_setTexture(new_part->star, c->textures.asteroid, False);
	sfCircleShape_setRotation(new_part->star, random_number(0, 360));
	new_part->next = (*head);
	new_part->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = new_part;
	(*head) = new_part;
	index++;
}

void del_star(stars_t **head, stars_t *del_node)
{
	if (*head == NULL || del_node == NULL)
		return ;
	if (*head == del_node)
		*head = del_node->next;
	if (del_node->next != NULL)
		del_node->next->prev = del_node->prev;
	if (del_node->prev != NULL)
		del_node->prev->next = del_node->next;
	sfCircleShape_destroy(del_node->star);
	free(del_node);
}
