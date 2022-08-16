/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** ships.c
*/

#include "my.h"

void orbit_target(core_t *c, ship_t *ship, sfVector2f target)
{
	float accel = ship->acceleration;

	if (dist_from(ship->pos, target) > 400) {
        ship->vect.x += cos(deg_to_rad(ship->angle)) * accel;
        ship->vect.y += sin(deg_to_rad(ship->angle)) * accel;
        ship->direction.forward = True;
        ship->direction.backward = False;
	}
	if (dist_from(ship->pos, target) < 350) {
        ship->vect.x += cos(deg_to_rad(ship->angle + 180)) * accel / 2;
        ship->vect.y += sin(deg_to_rad(ship->angle + 180)) * accel / 2;
        ship->direction.forward = False;
        ship->direction.backward = True;
	}
}

void update_exhaust(core_t *c, ship_t *ship)
{
    smoke_emitter_t *node = ship->model->exhausts;

    while (node != NULL) {
        node->dir.x = -cos(deg_to_rad(ship->angle)) * 0.8;
        node->dir.y = -sin(deg_to_rad(ship->angle)) * 0.8;
        node->pos = sprite_gethead(ship->model->sprite, node->relative_pos);
		node->angle = ship->angle;
        handle_smoke(c, node, vect_mag(ship->vect));
        node = node->next;
    }
}

int get_side(sfVector2f a, sfVector2f b, sfVector2f p)
{
    int position = (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
	if (position < 0)
	    return -1;
	if (position == 0)
	    return 0;
	if (position > 0)
	    return 1;
}

void avoid_allies(core_t *c, ship_t *ship)
{
	ship_t *tmp = c->entities.ships;
	sfFloatRect r1 = sfSprite_getGlobalBounds(ship->model->sprite);
	sfFloatRect r2;
	sfFloatRect r3;
	float radius1;
	float radius2;
	int side;
	int found = 0;
	float accel = ship->acceleration;

    r1.width > r1.height ? (radius1 = r1.width * 0.7) : (radius1 = r1.height * 0.7);
	while (tmp != NULL) {
		r2 = sfSprite_getGlobalBounds(tmp->model->sprite);
        r2.width > r2.height ? (radius2 = r2.width * 0.7) : (radius2 = r2.height * 0.7);
		if (circles_intersect(ship->pos, radius1, tmp->pos, radius2)
		    && ship->index != tmp->index && ship->direction.left == False) {
			side = get_side(ship->pos, sprite_gethead(ship->model->sprite, (sfVector2f){60, 0}), tmp->pos);
            if (side == 1) {
                ship->vect.x += cos(deg_to_rad(ship->angle + 90)) * accel / 4;
                ship->vect.y += sin(deg_to_rad(ship->angle + 90)) * accel / 4;
                tmp->vect.x += cos(deg_to_rad(tmp->angle - 90)) * accel / 4;
                tmp->vect.y += sin(deg_to_rad(tmp->angle - 90)) * accel / 4;
			} else if (side == -1) {
	            ship->vect.x += cos(deg_to_rad(ship->angle - 90)) * accel / 4;
                ship->vect.y += sin(deg_to_rad(ship->angle - 90)) * accel / 4;
                tmp->vect.x += cos(deg_to_rad(tmp->angle + 90)) * accel / 4;
                tmp->vect.y += sin(deg_to_rad(tmp->angle + 90)) * accel / 4;
			}
			ship->direction.right = True;
			tmp->direction.left = True;
			found = 1;
		}
		tmp = tmp->next;
	}
	if (found == 0) {
		ship->direction.left = False;
		ship->direction.right = False;
	}
}

model_t *new_model(core_t *c, int type)
{
    model_t *ship = malloc(sizeof(model_t));
	sfVector2f pos = {-70, 10};

    ship->sprite = sfSprite_create();
    ship->exhausts = NULL;
    add_exhaust(c, &ship->exhausts, pos);
	pos = (sfVector2f){-70, -10};
    add_exhaust(c, &ship->exhausts, pos);
    ship->gun.cooldown = 0.4;
    ship->gun.recoil = 1;
    sfSprite_setTexture(ship->sprite, c->textures.ships[type], sfTrue);
    sfSprite_setScale(ship->sprite, (sfVector2f){0.7, 0.7});
    sfSprite_setOrigin(ship->sprite, get_sprite_center(ship->sprite));
	return ship;
}

static shield_t create_shield(core_t *c)
{
	shield_t shield;

    shield.max_nrj = 200;
    shield.current_nrj = shield.max_nrj;
    shield.sprite = sfRectangleShape_create();
    shield.nrj_clock = 0;
    shield.nrj_reg_ammount = 2;
    shield.nrj_reg_speed = 0.1;
    shield.nrj_reg_cooldown_time = 3;
    shield.nrj_can_reg = true;
    shield.nrj_reg_cooldown_clock = 0;
    sfRectangleShape_setSize(shield.sprite, (sfVector2f){200, 200});
    sfRectangleShape_setOrigin(shield.sprite, get_rect_center(shield.sprite));
    sfRectangleShape_setTexture(shield.sprite, c->textures.shield, False);
	return shield;
}

void add_ship(core_t *c, ship_t **head, sfVector2f pos, int team, int type, bool player)
{
	static int index = 0;
	ship_t *nnode;

	nnode = malloc(sizeof(ship_t));
	nnode->index = index;
    nnode->acceleration = 0.5;
    nnode->shoot_clock = 0;
	nnode->team = team;
	nnode->integrity = 30;
	nnode->max_integrity = nnode->integrity;
	nnode->turn_speed = 4.5;
	nnode->triggered = False;
	nnode->shield = create_shield(c);
	nnode->player = player;
    nnode->speed = 5 + 5;
    nnode->angle = ((float)random_number(0, 3600) / 10);
    nnode->vect = (sfVector2f){0, 0};
	nnode->model = new_model(c, type);
	nnode->acceleration_max = nnode->acceleration;
    sfSprite_setPosition(nnode->model->sprite, pos);
    sfSprite_setRotation(nnode->model->sprite, nnode->angle);
	nnode->next = (*head);
	nnode->prev = NULL;
	if ((*head) != NULL)
		(*head)->prev = nnode;
	(*head) = nnode;
	index++;
}

void del_ship(ship_t **head, ship_t *del_node)
{
	if (*head == NULL || del_node == NULL)
		return ;
	if (*head == del_node)
		*head = del_node->next;
	if (del_node->next != NULL)
		del_node->next->prev = del_node->prev;
	if (del_node->prev != NULL)
		del_node->prev->next = del_node->next;
    sfSprite_destroy(del_node->model->sprite);
    free(del_node->model->exhausts);
	free(del_node->model);
	free(del_node);
}
