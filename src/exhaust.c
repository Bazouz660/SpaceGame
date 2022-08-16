/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** exhaust.c
*/

#include "my.h"

void add_exhaust(core_t *c, smoke_emitter_t **head, sfVector2f relative_pos)
{
	static int index = 0;
	smoke_emitter_t *new_part;

	new_part = malloc(sizeof(smoke_emitter_t));
    new_part->clock = c->clock.seconds;
    new_part->dir = (sfVector2f){0, 0};
    new_part->pos = relative_pos;
    new_part->relative_pos = relative_pos;
    new_part->parts = NULL;
	new_part->next = (*head);
	new_part->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = new_part;
	(*head) = new_part;
	index++;
}

void del_exhaust(particle_t **head, particle_t *del_node)
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
