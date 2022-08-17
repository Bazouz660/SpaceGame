/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** bolts.c
*/

#include "my.h"

void add_bolt(core_t *c, projectiles_t **head, ship_t *src)
{
	static int index = 0;
	projectiles_t *nnode;

	nnode = malloc(sizeof(projectiles_t));
    nnode->sprite = sfSprite_create();
	nnode->team = src->team;
	nnode->dmg = 10;
	sfSprite_setColor(nnode->sprite, sfRed);
    sfSprite_setTexture(nnode->sprite, c->textures.bolt, sfTrue);
    sfSprite_setOrigin(nnode->sprite, get_sprite_center(nnode->sprite));
    sfSprite_setPosition(nnode->sprite, sprite_gethead(src->model->sprite,
	(sfVector2f){sfSprite_getGlobalBounds(src->model->sprite).width, 0}));
    sfSprite_setRotation(nnode->sprite, src->angle);
    nnode->vect.x = (cos(deg_to_rad(src->angle)) * 25);
    nnode->vect.y = (sin(deg_to_rad(src->angle)) * 25);
	nnode->vect = vect_fmult(nnode->vect, 1 + ((vect_mag(src->vect) / 20) * \
	src->direction.forward));
	nnode->next = (*head);
	nnode->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = nnode;
	(*head) = nnode;
	index++;
}

void del_bolt(projectiles_t **head, projectiles_t *del_node)
{
	if (*head == NULL || del_node == NULL)
		return ;
	if (*head == del_node)
		*head = del_node->next;
	if (del_node->next != NULL)
		del_node->next->prev = del_node->prev;
	if (del_node->prev != NULL)
		del_node->prev->next = del_node->next;
    sfSprite_destroy(del_node->sprite);
	free(del_node);
}
