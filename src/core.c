/*
** EPITECH PROJECT, 2021
** B-MUL-200-STG-2-1-myrpg-basile.trebus-hamann
** File description:
** main.c
*/

#include "my.h"

void zoom(core_t *c)
{
    if (c->event.type == sfEvtMouseWheelMoved) {
        if (c->event.mouseWheel.delta == -1 && c->render.zoom_factor < Z_MAX) {
            c->render.zoom_factor *= 1.03;
            sfView_zoom(c->render.view, 1.03);
            sfRenderWindow_setView(c->render.window, c->render.view);
        }
        if (c->event.mouseWheel.delta == 1 && c->render.zoom_factor > Z_MIN) {
            c->render.zoom_factor *= 0.97;
            sfView_zoom(c->render.view, 0.97);
            sfRenderWindow_setView(c->render.window, c->render.view);
        }
        sfRectangleShape_setScale(c->shader.postproc_rect,
        (sfVector2f){c->render.zoom_factor, c->render.zoom_factor});
    }
}

void analyse_events(core_t *c)
{
    while (sfRenderWindow_pollEvent(c->render.window, &c->event)) {
        if (c->event.type == sfEvtClosed)
            sfRenderWindow_close(c->render.window);
        if (sfKeyboard_isKeyPressed(sfKeyEscape))
            sfRenderWindow_close(c->render.window);
        zoom(c);
    }
}

void update_clock(core_t *c)
{
    c->clock.time = sfClock_getElapsedTime(c->clock.clock);
    c->clock.seconds = c->clock.time.microseconds/1000000.0;
}

void update_fps(core_t *c)
{
    float fps = get_fps(c->clock.clock, &c->clock.prev_time);

    if (c->clock.seconds > c->render.fps_clock + 0.3) {
        c->render.fps_clock = c->clock.seconds;
        sfText_setString(c->render.fps_hint, my_int_to_str(fps));
    }
}

void draw_stars(core_t *c)
{
    stars_t *tmp = c->skybox;
    sfVector2f pos;
    sfFloatRect bounds = {SP_LEFT, SP_TOP, SP_WIDTH, SP_HEIGHT};

    while (tmp != NULL) {
        sfRenderWindow_drawCircleShape(c->render.window, tmp->star, NULL);
        pos = sfCircleShape_getPosition(tmp->star);
        if (((pos.x < bounds.left - bounds.width && c->player->vect.x > 0 )
            || (pos.x > bounds.left + (bounds.width * 2)
            && c->player->vect.x < 0))
            || ((pos.y < bounds.top - bounds.height && c->player->vect.y > 0)
            || (pos.y > bounds.top + (bounds.height * 2)
            && c->player->vect.y < 0))) {
            randomise_spawns(c);
            del_star(&c->skybox, tmp);
            add_star(c, &c->skybox, True);
        }
        tmp = tmp->next;
    }
}

void draw_bolts(core_t *c, projectiles_t *bolts)
{
    while (bolts != NULL) {
        sfRenderWindow_drawSprite(c->render.window, bolts->sprite, NULL);
        bolts = bolts->next;
    }
}

void draw_ships(core_t *c)
{
    ship_t *tmp = c->entities.ships;
    smoke_emitter_t *node;
    int x = 0;

    while (tmp != NULL) {
        node = tmp->model->exhausts;
        while (node != NULL) {
            node->dir.x = -cos(deg_to_rad(tmp->angle)) * 0.8;
            node->dir.y = -sin(deg_to_rad(tmp->angle)) * 0.8;
            node->pos = sprite_gethead(tmp->model->sprite, node->relative_pos);
            node->angle = tmp->angle;
            handle_smoke(c, node, vect_mag(tmp->vect));
            draw_smoke(c, node->parts);
            node = node->next;
        }
        sfRenderWindow_drawSprite(c->render.window, tmp->model->sprite, NULL);
        tmp = tmp->next;
    }
}

void draw_explosions(core_t *c)
{
    explosion_t *tmp = c->entities.explosions;

    while (tmp != NULL) {
        render_smoke(c, &tmp->parts, tmp->parts);
        draw_smoke(c, tmp->parts);
        tmp = tmp->next;
    }
}

void rect_set_transluscent(sfRectangleShape *rect, float procentage)
{
    sfColor color = sfRectangleShape_getFillColor(rect);

    if (procentage > 1)
        procentage = 1;
    if (procentage < 0) {
        procentage = 0;
    }
    color.a = 255 * procentage;
    sfRectangleShape_setFillColor(rect, color);
}

float get_sound_mid(visualizer_t *vz)
{
    float mid;
    int j = 0;

    j = sfTime_asMilliseconds(sfSound_getPlayingOffset(vz->song)) * 88.15;
    if(j > 500) {
        mid = 0;
        for(int i = -500; i < 500; i++) {
            mid += abs(vz->samples[j - i]);
        }
        mid /= 1000;
    }
    return mid;
}

void apply_bloom(core_t *c, float intensity, float threshold)
{
    float o = 0.001;
    //static float z = 0;
    static int x = 0;
    static float s = 0.01;
    sfRectangleShape_setPosition(c->shader.postproc_rect, sfView_getCenter(c->render.view));
    sfRectangleShape_setTexture(c->shader.postproc_rect, c->shader.postproc_txtr, False);
    sfShader_setFloatUniform(c->shader.luminescence, "threshold", threshold);
    sfShader_setFloatUniform(c->shader.boxblur, "texture_inverse", 1.0 / c->render.size.x);
    sfShader_setIntUniform(c->shader.boxblur, "blur_radius", 20);
    sfShader_setFloatUniform(c->shader.assemble, "add_weight", 1);
    sfShader_setFloatUniform(c->shader.multiply, "multiply", intensity);

    c->shader.render_states.shader = c->shader.luminescence;

    sfTexture_updateFromRenderWindow(c->shader.f_buffer1, c->render.window, 0, 0);

    sfTexture_updateFromRenderWindow(c->shader.postproc_txtr, c->render.window, 0, 0);
    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);

    c->shader.render_states.shader = c->shader.boxblur;

    sfTexture_updateFromRenderWindow(c->shader.postproc_txtr, c->render.window, 0, 0);
    sfShader_setVec2Uniform(c->shader.boxblur, "blur_direction", (sfGlslVec2){0, 2});
    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);
    sfTexture_updateFromRenderWindow(c->shader.postproc_txtr, c->render.window, 0, 0);
    sfShader_setVec2Uniform(c->shader.boxblur, "blur_direction", (sfGlslVec2){1, 0});
    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);
    sfTexture_updateFromRenderWindow(c->shader.postproc_txtr, c->render.window, 0, 0);
    c->shader.render_states.shader = c->shader.multiply;
    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);
    sfTexture_updateFromRenderWindow(c->shader.f_buffer2, c->render.window, 0, 0);

    sfRectangleShape_setTexture(c->shader.postproc_rect, c->shader.f_buffer1, False);

    sfShader_setTextureUniform(c->shader.assemble, "add_texture", c->shader.f_buffer2);

    c->shader.render_states.shader = c->shader.assemble;

    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);
}

void draw_shields(core_t *c)
{
    ship_t *ship = c->entities.ships;
    static float z = 0;
    static float x = 0;
    float s;

    sfTexture_updateFromRenderWindow(c->shader.f_buffer3, c->render.window, 0, 0);
    sfRenderWindow_clear(c->render.window, sfTransparent);
    while (ship != NULL) {
        s = 0.01 + ((0.01 - ((ship->shield.current_nrj / ship->shield.max_nrj) / 100)) * 5);
        sfRectangleShape_setPosition(ship->shield.sprite, sfSprite_getPosition(ship->model->sprite));
        sfRectangleShape_setRotation(ship->shield.sprite, ship->angle);
        sfRenderWindow_drawRectangleShape(c->render.window, ship->shield.sprite, NULL);
        ship = ship->next;
    }
    if (z >= 1)
        x = 1;
    if (z <= 0)
        x = 0;
    if (x == 0)
        z += s;
    else if (x == 1)
        z -= s;
    apply_bloom(c, z, 0.0);
    sfTexture_updateFromRenderWindow(c->shader.f_buffer1, c->render.window, 0, 0);
    sfShader_setTextureUniform(c->shader.assemble, "add_texture", c->shader.f_buffer3);
    sfRenderWindow_drawRectangleShape(c->render.window, c->shader.postproc_rect, &c->shader.render_states);
}

void rect_texrect_proc(sfRectangleShape *shape, float procentage)
{
    sfVector2u t_size = sfTexture_getSize(sfRectangleShape_getTexture(shape));
    sfIntRect t_rect;
    sfVector2f scale = sfRectangleShape_getScale(shape);

    t_rect.left = 0;
    t_rect.top = 0;
    t_rect.width = t_size.x * procentage;
    t_rect.height = t_size.y;

    sfRectangleShape_setTextureRect(shape, t_rect);
    sfRectangleShape_setScale(shape, (sfVector2f){scale.x * procentage, scale.y});
}

void draw_hud(core_t *c)
{
    sfVector2f pos;

    pos.x = view_bounds(c->render.view).left + (view_bounds(c->render.view).width / 7.2);
    pos.y = view_bounds(c->render.view).top + (view_bounds(c->render.view).height / 1.115);
    sfSprite_setPosition(c->hud.shield_bar_out, pos);
    pos.x = view_bounds(c->render.view).left + (view_bounds(c->render.view).width / 9.2);
    pos.y = view_bounds(c->render.view).top + (view_bounds(c->render.view).height / 1.153);
    sfRectangleShape_setPosition(c->hud.shield_bar_in, pos);
    pos.x = view_bounds(c->render.view).left + (view_bounds(c->render.view).width / 9.75);
    pos.y = view_bounds(c->render.view).top + (view_bounds(c->render.view).height / 1.1905);
    rect_texrect_proc(c->hud.health_bar_in, c->player->integrity / c->player->max_integrity);
    sfRectangleShape_setPosition(c->hud.health_bar_in, pos);
    sfRectangleShape_setScale(c->hud.shield_bar_in,
    (sfVector2f){(c->player->shield.current_nrj / c->player->shield.max_nrj) \
    * c->render.zoom_factor, c->render.zoom_factor});
    sfRenderWindow_drawSprite(c->render.window, c->hud.shield_bar_out, NULL);
    sfRenderWindow_drawRectangleShape(c->render.window, c->hud.shield_bar_in, NULL);
    sfRenderWindow_drawRectangleShape(c->render.window, c->hud.health_bar_in, NULL);
}

void draw_all(core_t *c)
{
    sfRenderWindow_display(c->render.window);
    sfRenderWindow_clear(c->render.window, sfTransparent);
    c->shader.render_states.shader = c->shader.boxblur;
    draw_stars(c);
    draw_bolts(c, c->entities.bolts);
    draw_smoke(c, c->player->model->exhausts->parts);
    draw_ships(c);
    draw_explosions(c);
    draw_shields(c);
    sfSprite_setPosition(c->render.cursor, get_mouse_pos_view(c));
    sfRenderWindow_drawSprite(c->render.window, c->render.cursor, NULL);
    sfRenderWindow_drawText(c->render.window, c->hud.shield_nrj_txt, NULL);
    draw_hud(c);
    apply_bloom(c, 0.8, 0.5);
    sfRenderWindow_drawText(c->render.window, c->render.fps_hint, NULL);
}

sfVector2f sprite_gethead(sfSprite *sprite, sfVector2f offset)
{
    float s = sin(deg_to_rad(sfSprite_getRotation(sprite)));
    float c = cos(deg_to_rad(sfSprite_getRotation(sprite)));
    float xnew;
    float ynew;
    sfVector2f p = sfSprite_getPosition(sprite);
    sfVector2f p2 = p;

    p.x += offset.x;
    p.y += offset.y;
    p.x -= p2.x;
    p.y -= p2.y;
    xnew = p.x * c - p.y * s;
    ynew = p.x * s + p.y * c;
    p.x = xnew + p2.x;
    p.y = ynew + p2.y;
    return p;
}

void player_controller(core_t *c)
{
    float accel = c->player->acceleration;

    if (sfKeyboard_isKeyPressed(sfKeyZ)) {
        c->player->vect.x += cos(deg_to_rad(c->player->angle)) * accel;
        c->player->vect.y += sin(deg_to_rad(c->player->angle)) * accel;
        c->player->direction.forward = True;
        c->player->direction.backward = False;
    } else if (sfKeyboard_isKeyPressed(sfKeyS)) {
        c->player->vect.x += cos(deg_to_rad(c->player->angle + 180)) * accel / 2;
        c->player->vect.y += sin(deg_to_rad(c->player->angle + 180)) * accel / 2;
        c->player->direction.forward = False;
        c->player->direction.backward = True;
    }
    if (sfKeyboard_isKeyPressed(sfKeyD)) {
        c->player->vect.x += cos(deg_to_rad(c->player->angle + 90)) * accel * 0.8;
        c->player->vect.y += sin(deg_to_rad(c->player->angle + 90)) * accel * 0.8;
        c->player->direction.right = True;
        c->player->direction.left = False;
    } else if (sfKeyboard_isKeyPressed(sfKeyQ)) {
        c->player->vect.x += cos(deg_to_rad(c->player->angle - 90)) * accel * 0.8;
        c->player->vect.y += sin(deg_to_rad(c->player->angle - 90)) * accel * 0.8;
        c->player->direction.right = False;
        c->player->direction.left = True;
    }
}

void update_exhausts(core_t *c)
{
    smoke_emitter_t *node = c->player->model->exhausts;

    while (node != NULL) {
        node->dir.x = -cos(deg_to_rad(c->player->angle)) * 0.8;
        node->dir.y = -sin(deg_to_rad(c->player->angle)) * 0.8;
        node->pos = sprite_gethead(c->player->model->sprite, (sfVector2f){-70, 0});
        node->angle = c->player->angle;
        handle_smoke(c, node, vect_mag(c->player->vect));
        node = node->next;
    }
}

void play_thruster_sound(core_t *c)
{
    static float volume = 0;

    if ((c->player->vect.x > 1 || c->player->vect.x < -1)
        || (c->player->vect.y > 1 || c->player->vect.y < -1)) {
        volume += 2;
        if (volume >= 70)
            volume = 70;
        if (sfSound_getStatus(c->sounds.thruster) != 2)
            sfSound_play(c->sounds.thruster);
    } else {
        volume -= 3;
        if (volume <= 0)
            volume = 0;
    }
    sfSound_setVolume(c->sounds.thruster, volume);
}

void shoot(core_t *c)
{
    projectiles_t *tmp;

    if (c->clock.seconds > c->player->shoot_clock + c->player->model->gun.cooldown) {
        add_bolt(c, &c->entities.bolts, c->player);
        c->player->shoot_clock = c->clock.seconds;
        modulate_sound(c->sounds.blaster);
        sfSound_play(c->sounds.blaster);
        c->player->vect.x += cos(deg_to_rad(c->player->angle + 180) * c->player->model->gun.recoil);
        c->player->vect.y += sin(deg_to_rad(c->player->angle + 180) * c->player->model->gun.recoil);
    }
}

sfColor smooth_color(sfColor c1, sfColor c2, double prog)
{
    float diff_r = c1.r - c2.r;
    float diff_g = c1.g - c2.g;
    float diff_b = c1.b - c2.b;
    float diff_a = c1.a - c2.a;
    sfColor res;

    if (prog > 1)
        prog = 1;
    if (prog < 0)
        prog = 0;
    res.r = c1.r + (diff_r * prog);
    res.g = c1.g + (diff_g * prog);
    res.b = c1.b + (diff_b * prog);
    res.a = c1.a + (diff_a * prog);
    return res;
}

void change_shield_color(core_t *c, shield_t *shield)
{
    if (shield->current_nrj == shield->max_nrj) {
        sfRectangleShape_setFillColor(shield->sprite, sfCyan);
        return;
    }
    if (shield->current_nrj <= 0) {
        shield->current_nrj = 0;
        if (sfRectangleShape_getFillColor(shield->sprite).a != 0) {
            sfSound_play(c->sounds.shield_down);
        }
        sfRectangleShape_setFillColor(shield->sprite, sfTransparent);
        return;
    } else if (shield->nrj_can_reg == false) {
        sfRectangleShape_setFillColor(shield->sprite, smooth_color(sfGreen, sfRed, (double)(shield->current_nrj / shield->max_nrj)));
        return;
    }
}

void update_shield(core_t *c, shield_t *shield)
{
    change_shield_color(c, shield);
    if (c->clock.seconds > shield->nrj_reg_cooldown_clock
        + shield->nrj_reg_cooldown_time && shield->nrj_can_reg == false) {
        shield->nrj_reg_cooldown_clock = c->clock.seconds;
        if (shield->current_nrj <= 0)
            sfSound_play(c->sounds.shield_boot);
        shield->nrj_can_reg = true;
    }
    if ((shield->current_nrj < shield->max_nrj)
        && (c->clock.seconds > shield->nrj_clock + shield->nrj_reg_speed)
        && shield->nrj_can_reg == true) {
        shield->nrj_clock = c->clock.seconds;
        shield->current_nrj += shield->nrj_reg_ammount;
    }
    if (shield->current_nrj > shield->max_nrj)
        shield->current_nrj = shield->max_nrj;
}

void check_player_hit(core_t *c)
{
    projectiles_t *prjt = c->entities.bolts;
    sfFloatRect r1 = sfSprite_getGlobalBounds(c->player->model->sprite);
    sfFloatRect r2;

    while (prjt != NULL) {
        r2 = sfSprite_getGlobalBounds(prjt->sprite);
        if (sfFloatRect_intersects(&r1, &r2, NULL)
            && prjt->team != PLAYER_TEAM) {
            if (c->player->shield.current_nrj <= 0) {
                c->player->integrity -= prjt->dmg;
                c->player->integrity < 0 ? c->player->integrity = 0 : 0;
            }
            c->player->shield.current_nrj -= prjt->dmg;
            if (c->player->shield.current_nrj > 0) {
                modulate_sound(c->sounds.shield_hit);
                sfSound_play(c->sounds.shield_hit);
            } else {
                modulate_sound(c->sounds.impact);
                sfSound_play(c->sounds.impact);
            }
            c->player->shield.nrj_can_reg = false;
            c->player->shield.nrj_reg_cooldown_clock = c->clock.seconds;
            del_bolt(&c->entities.bolts, prjt);
        }
        prjt = prjt->next;
    }
    update_shield(c, &c->player->shield);
}

void update_player(core_t *c, ship_t *ship)
{
    find_target(c, ship, c->mouse.pos);
    player_controller(c);
    engage_target(c, ship);
    play_thruster_sound(c);
    sfText_setString(c->hud.shield_nrj_txt,
    my_strcat(my_int_to_str((c->player->shield.current_nrj / \
    c->player->shield.max_nrj) * 100), "%"));
}

void update_view(core_t *c)
{
    c->render.view_pos = vect_add(c->render.view_pos, vect_fmult(vect_sub(c->player->pos, c->render.view_pos), 0.2));
    sfView_setCenter(c->render.view, c->render.view_pos);
    sfRenderWindow_setView(c->render.window, c->render.view);
}

sfVector2f get_text_center(sfText *text)
{
    sfFloatRect rect = sfText_getLocalBounds(text);
    sfVector2f center;

    center.x = rect.width / 2;
    center.y = rect.height / 2;
    return center;
}

void scale_with_zoom(core_t *c)
{
    sfVector2f scale = {c->render.zoom_factor, c->render.zoom_factor};

    sfText_setScale(c->render.fps_hint, vect_fmult(scale, 1));
    sfText_setScale(c->hud.shield_nrj_txt, vect_fmult(scale, 1));
    sfRectangleShape_setScale(c->hud.health_bar_in, scale);
    sfSprite_setScale(c->render.cursor, vect_fmult(scale, 0.07));
    sfSprite_setScale(c->hud.shield_bar_out, vect_fmult(scale, 0.61));
    sfRectangleShape_setScale(c->hud.shield_bar_in, scale);
}

void move_all(core_t *c)
{
    sfVector2f pos;
    ship_t *ship = c->entities.ships;

    scale_with_zoom(c);
    pos.x = view_bounds(c->render.view).left + (view_bounds(c->render.view).width / 20);
    pos.y = view_bounds(c->render.view).top + (view_bounds(c->render.view).height / 20);
    sfText_setPosition(c->render.fps_hint, pos);
    pos.x = view_bounds(c->render.view).left + (view_bounds(c->render.view).width / 17.65);
    pos.y = view_bounds(c->render.view).top + (view_bounds(c->render.view).height / 1.125);
    sfText_setPosition(c->hud.shield_nrj_txt, pos);
    sfText_setOrigin(c->hud.shield_nrj_txt, get_text_center(c->hud.shield_nrj_txt));
    while (ship != NULL) {
        ship->vect = clamp_vect2f(ship->vect, ship->speed);
        sfSprite_move(ship->model->sprite, ship->vect);
        ship->vect.x *= 0.97;
        ship->vect.y *= 0.97;
        ship = ship->next;
    }
}

float find_target(core_t *c, ship_t *ship, sfVector2f target)
{
    float a = get_angle(ship->pos, target);
    float diff;
    float t_diff;

    ship->pos = sfSprite_getPosition(ship->model->sprite);
    diff = a - ship->angle;
    if (diff > 180)
        diff = (a - 360) - ship->angle;
    else if (diff < -180)
        diff = (a + 360) - ship->angle;
    if (diff > 40)
        diff = 40;
    if (diff < -40)
        diff = -40;
    t_diff = diff;
    diff *= ship->turn_speed / 100;
    if (diff > 0)
        ship->angle += (diff * diff * diff) + 0.5;
    if (diff < 0) {
        diff *= -1;
        ship->angle -= (diff * diff * diff) + 0.5;
    }
    sfSprite_setRotation(ship->model->sprite, ship->angle);
    ship->angle = sfSprite_getRotation(ship->model->sprite);
    return t_diff;
}

void engage_target(core_t *c, ship_t *ship)
{
    projectiles_t *tmp;

    if (c->clock.seconds > ship->shoot_clock + ship->model->gun.cooldown) {
        if (((sfKeyboard_isKeyPressed(sfKeySpace) ||
            sfMouse_isButtonPressed(sfMouseLeft)) && ship->player == true)
            || ship->player == false) {
            add_bolt(c, &c->entities.bolts, ship);
            ship->shoot_clock = c->clock.seconds;
            modulate_sound(c->sounds.blaster);
            sfSound_play(c->sounds.blaster);
            ship->vect.x += cos(deg_to_rad(ship->angle + 180) *\
            ship->model->gun.recoil);
            ship->vect.y += sin(deg_to_rad(ship->angle + 180) *\
            ship->model->gun.recoil);
        }
    }
}

void check_damage(core_t *c, ship_t *ship)
{
    projectiles_t *prjt = c->entities.bolts;
    sfFloatRect r1 = sfSprite_getGlobalBounds(ship->model->sprite);
    sfFloatRect r2;

    while (prjt != NULL) {
        r2 = sfSprite_getGlobalBounds(prjt->sprite);
        if (sfFloatRect_intersects(&r1, &r2, NULL)
            && prjt->team != ship->team) {
            ship->triggered = True;
            if (ship->shield.current_nrj <= 0) {
                ship->integrity -= prjt->dmg;
                ship->integrity < 0 ? ship->integrity = 0 : 0;
            }
            ship->shield.current_nrj -= prjt->dmg;
            if (ship->shield.current_nrj > 0) {
                modulate_sound(c->sounds.shield_hit);
                sfSound_play(c->sounds.shield_hit);
            } else {
                modulate_sound(c->sounds.impact);
                sfSound_play(c->sounds.impact);
            }
            ship->shield.nrj_can_reg = false;
            ship->shield.nrj_reg_cooldown_clock = c->clock.seconds;
            del_bolt(&c->entities.bolts, prjt);
        }
        prjt = prjt->next;
    }
    update_shield(c, &ship->shield);
}

void update_ships(core_t *c)
{
    ship_t *ship;
    sfVector2f target_pos = {c->player->pos.x + c->player->vect.x,
    c->player->pos.y + c->player->vect.y};
    sfFloatRect bounds = view_bounds(c->render.view);

    if (c->entities.ships_nb < MAX_SHIP_NB) {
        add_ship(c, &c->entities.ships,
        c->entities.spawn_pos[random_number(0, 7)], 1,
        random_number(0, 7), false);
        c->entities.ships_nb++;
    }
    ship = c->entities.ships;
    while (ship != NULL) {
        update_exhaust(c, ship);
        ship->pos = sfSprite_getPosition(ship->model->sprite);
        if ((dist_from(ship->pos, c->player->pos) < 1200
            || ship->triggered == True) && (ship->player == false)) {
            if (absolute(find_target(c, ship, target_pos)) < 20)
                orbit_target(c, ship, target_pos);
        }
        if ((dist_from(ship->pos, c->player->pos) < 800
            || ship->triggered == true) && ship->player == false) {
            engage_target(c, ship);
        }
        if (ship->player == true) {
            update_player(c, ship);
        }
        avoid_allies(c, ship);
        check_damage(c, ship);
        if ((dist_from(ship->pos, c->player->pos) > \
        (sfRenderWindow_getSize(c->render.window).x * 2)
            || ship->integrity <= 0) && ship->player == False) {
            if (ship->integrity <= 0)
                new_explosion(c, &c->entities.explosions, ship->pos, 20);
            del_ship(&c->entities.ships, ship);
            c->entities.ships_nb -= 1;
        }
        ship = ship->next;
    }
}

void update_bolts(core_t *c)
{
    sfFloatRect bounds = view_bounds(c->render.view);
    projectiles_t *tmp = c->entities.bolts;

    while (tmp != NULL) {
        sfSprite_move(tmp->sprite, tmp->vect);
        if (!sfFloatRect_contains(&bounds, sfSprite_getPosition(tmp->sprite).x,
            sfSprite_getPosition(tmp->sprite).y)) {
            del_bolt(&c->entities.bolts, tmp);
        }
        tmp = tmp->next;
    }
}

void render_game(core_t *c)
{
    c->mouse.pos = get_mouse_pos_view(c);
    update_clock(c);
    randomise_spawns(c);
    update_fps(c);
    update_bolts(c);
    update_view(c);
    update_ships(c);
    move_all(c);
    draw_all(c);
    analyse_events(c);
}

void randomise_spawns(core_t *c)
{
    c->entities.spawn_pos[0].x = random_number(SP_LEFT - SP_WIDTH, SP_LEFT);
    c->entities.spawn_pos[0].y = random_number(SP_TOP - SP_HEIGHT, SP_TOP);
    c->entities.spawn_pos[1].x = random_number(SP_LEFT, SP_LEFT + SP_WIDTH);
    c->entities.spawn_pos[1].y = random_number(SP_TOP - SP_HEIGHT, SP_TOP);
    c->entities.spawn_pos[2].x = random_number(SP_LEFT + SP_WIDTH, SP_LEFT\
    + (SP_WIDTH) * 2);
    c->entities.spawn_pos[2].y = random_number(SP_TOP - SP_HEIGHT, SP_TOP);
    c->entities.spawn_pos[3].x = random_number(SP_LEFT - SP_WIDTH, SP_LEFT);
    c->entities.spawn_pos[3].y = random_number(SP_TOP, SP_TOP + SP_HEIGHT);
    c->entities.spawn_pos[4].x = random_number(SP_LEFT + SP_WIDTH, SP_LEFT\
    + (SP_WIDTH) * 2);
    c->entities.spawn_pos[4].y = random_number(SP_TOP, SP_TOP + SP_HEIGHT);
    c->entities.spawn_pos[5].x = random_number(SP_LEFT - SP_WIDTH, SP_LEFT);
    c->entities.spawn_pos[5].y = random_number(SP_TOP + SP_HEIGHT, SP_TOP\
    + (SP_HEIGHT) * 2);
    c->entities.spawn_pos[6].x = random_number(SP_LEFT, SP_LEFT + SP_WIDTH);
    c->entities.spawn_pos[6].y = random_number(SP_TOP + SP_HEIGHT, SP_TOP\
    + (SP_HEIGHT) * 2);
    c->entities.spawn_pos[7].x = random_number(SP_LEFT + SP_WIDTH, SP_LEFT\
    + (SP_WIDTH) * 2);
    c->entities.spawn_pos[7].y = random_number(SP_TOP + SP_HEIGHT, SP_TOP\
    + (SP_HEIGHT) * 2);
    c->entities.spawn_pos[8].x = random_number(SP_LEFT, SP_LEFT + SP_WIDTH);
    c->entities.spawn_pos[8].y = random_number(SP_TOP, SP_TOP + SP_HEIGHT);
}

int game_loop(void)
{
    core_t *c = malloc(sizeof(core_t));

    init_game(c);
    print_found_files();
    sfRenderWindow_setVerticalSyncEnabled(c->render.window, sfTrue);
    while (sfRenderWindow_isOpen(c->render.window))
        render_game(c);
}

int main(int ac, char **av)
{
    return game_loop();
}
