/*
** EPITECH PROJECT, 2022
** SpaceGame
** File description:
** init.c
*/

#include "my.h"

void init_fps_text(core_t *c)
{
    c->render.fps_hint = sfText_create();
    c->render.fps_clock = 0;
    c->render.font = sfFont_createFromFile("misc/font.ttf");
    sfText_setFont(c->render.fps_hint, c->render.font);
    sfText_setColor(c->render.fps_hint, sfWhite);
    sfText_setPosition(c->render.fps_hint, (sfVector2f){30, 30});
    sfText_setString(c->render.fps_hint, NULL);
}

void init_player(core_t *c)
{
    ship_t *tmp;
    c->player = malloc(sizeof(ship_t));

    add_ship(c, &c->entities.ships, ((sfVector2f){900, 500}), PLAYER_TEAM, 3, true);
    tmp = c->entities.ships;
    while (tmp != NULL) {
        if (tmp->player == true) {
            c->player = tmp;
        }
        tmp = tmp->next;
    }
}

void init_view(core_t *c)
{
    sfView_setSize(c->render.view,
    (sfVector2f){sfRenderWindow_getSize(c->render.window).x,
    sfRenderWindow_getSize(c->render.window).y});
    sfView_setCenter(c->render.view, c->player->pos);
    sfRenderWindow_setView(c->render.window, c->render.view);
    c->render.zoom_factor = 1;
    c->render.size = sfRenderWindow_getSize(c->render.window);
}

void init_sounds(core_t *c)
{
    c->sounds.thruster = set_up_new_sound(c->sounds.thruster,
    "sounds/ship/thruster.ogg");
    sfSound_setLoop(c->sounds.thruster, True);
    sfSound_setVolume(c->sounds.thruster, 70);
    c->sounds.blaster = set_up_new_sound(c->sounds.blaster,
    "sounds/guns/blaster.ogg");
    sfSound_setLoop(c->sounds.blaster, False);
    sfSound_setVolume(c->sounds.blaster, 30);
    c->sounds.impact = set_up_new_sound(c->sounds.impact,
    "sounds/sfx/impact.ogg");
    sfSound_setLoop(c->sounds.impact, False);
    sfSound_setVolume(c->sounds.impact, 50);
    c->sounds.shield_hit = set_up_new_sound(c->sounds.shield_hit,
    "sounds/sfx/shield_hit.ogg");
    c->sounds.shield_boot = set_up_new_sound(c->sounds.shield_hit,
    "sounds/sfx/shield_boot.ogg");
    c->sounds.shield_down = set_up_new_sound(c->sounds.shield_hit,
    "sounds/sfx/shield_down.ogg");
    c->sounds.explosion = sfSoundBuffer_createFromFile("sounds/sfx/explosion.ogg");
}

void init_sky(core_t *c)
{
    for (int i = 0; i < 200; i++) {
        randomise_spawns(c);
        add_star(c, &c->skybox, False);
    }
}

void init_textures(core_t *c)
{
    c->textures.smoke = sfTexture_createFromFile("sprites/sfx/smoke.png", NULL);
    c->textures.explosion = sfTexture_createFromFile("sprites/sfx/explosion.png", NULL);
    c->textures.shield_bar_out = sfTexture_createFromFile("sprites/hud/hudbar1.png", NULL);
    c->textures.shield_bar_in = sfTexture_createFromFile("sprites/hud/hudbar1ful.png", NULL);
    c->textures.health_bar_in = sfTexture_createFromFile("sprites/hud/hudbar2ful.png", NULL);
    c->textures.crosshair = sfTexture_createFromFile("sprites/player/scifi_crosshair.png", NULL);
    c->textures.player = sfTexture_createFromFile("sprites/player/ship.png", NULL);
    c->textures.bolt = sfTexture_createFromFile("sprites/guns/bolt.png", NULL);
    c->textures.shield = sfTexture_createFromFile("sprites/sfx/shield.png", NULL);
    c->textures.ships[0] = sfTexture_createFromFile("sprites/ships/1.png", NULL);
    c->textures.ships[1] = sfTexture_createFromFile("sprites/ships/2.png", NULL);
    c->textures.ships[2] = sfTexture_createFromFile("sprites/ships/3.png", NULL);
    c->textures.ships[3] = sfTexture_createFromFile("sprites/ships/4.png", NULL);
    c->textures.ships[4] = sfTexture_createFromFile("sprites/ships/5.png", NULL);
    c->textures.ships[5] = sfTexture_createFromFile("sprites/ships/6.png", NULL);
    c->textures.ships[6] = sfTexture_createFromFile("sprites/ships/7.png", NULL);
    c->textures.ships[7] = sfTexture_createFromFile("sprites/ships/8.png", NULL);
    c->textures.asteroid = sfTexture_createFromFile("sprites/environment/asteroid.png", NULL);
    c->textures.debris[0] = sfTexture_createFromFile("sprites/debris/part4.png", NULL);
    c->textures.debris[1] = sfTexture_createFromFile("sprites/debris/part5.png", NULL);
    c->textures.debris[2] = sfTexture_createFromFile("sprites/debris/part6.png", NULL);
    c->textures.debris[3] = sfTexture_createFromFile("sprites/debris/part1.png", NULL);
    c->textures.debris[4] = sfTexture_createFromFile("sprites/debris/part2.png", NULL);
    c->textures.debris[5] = sfTexture_createFromFile("sprites/debris/part3.png", NULL);
}

void init_hud(core_t *c)
{
    c->hud.shield_bar_out = sfSprite_create();
    sfSprite_setTexture(c->hud.shield_bar_out, c->textures.shield_bar_out, false);
    sfSprite_setOrigin(c->hud.shield_bar_out, get_sprite_center(c->hud.shield_bar_out));
    sfSprite_setScale(c->hud.shield_bar_out, (sfVector2f){0.61, 0.61});
    c->hud.shield_bar_in = sfRectangleShape_create();
    sfRectangleShape_setSize(c->hud.shield_bar_in, (sfVector2f){293, 43});
    sfRectangleShape_setTexture(c->hud.shield_bar_in, c->textures.shield_bar_in, false);
    c->hud.health_bar_in = sfRectangleShape_create();
    sfRectangleShape_setSize(c->hud.health_bar_in, (sfVector2f){216, 21});
    sfRectangleShape_setTexture(c->hud.health_bar_in, c->textures.health_bar_in, false);
    c->hud.shield_nrj_txt = sfText_create();
    sfText_setFont(c->hud.shield_nrj_txt, c->render.font);
    sfText_setFillColor(c->hud.shield_nrj_txt, sfCyan);
    sfText_setString(c->render.fps_hint, NULL);
    sfText_setCharacterSize(c->hud.shield_nrj_txt, 40);
}

void init_shaders(core_t *c)
{
    c->shader.luminescence = sfShader_createFromFile(NULL, NULL,
    "shaders/luminescence.frag");
    c->shader.multiply = sfShader_createFromFile(NULL, NULL,
    "shaders/multiply.frag");
    c->shader.assemble = sfShader_createFromFile(NULL, NULL,
    "shaders/assemble.frag");
    c->shader.boxblur = sfShader_createFromFile(NULL, NULL,
    "shaders/boxblur.frag");
    c->shader.render_states.blendMode = sfBlendNone;
    c->shader.render_states.shader = c->shader.boxblur;
    c->shader.render_states.texture = NULL;
    c->shader.render_states.transform = sfTransform_Identity;
    c->shader.postproc_txtr = sfTexture_create(c->render.size.x,
    c->render.size.y);
    c->shader.f_buffer1 = sfTexture_copy(c->shader.postproc_txtr);
    c->shader.f_buffer2 = sfTexture_copy(c->shader.f_buffer1);
    c->shader.f_buffer3 = sfTexture_copy(c->shader.f_buffer1);
    c->shader.postproc_rect = sfRectangleShape_create();
    sfRectangleShape_setSize(c->shader.postproc_rect,
    vect2u_vect2f(c->render.size));
    sfRectangleShape_setTexture(c->shader.postproc_rect,
    c->shader.postproc_txtr, sfTrue);
    sfRectangleShape_setOrigin(c->shader.postproc_rect,
    get_rect_center(c->shader.postproc_rect));
    sfRectangleShape_setPosition(c->shader.postproc_rect,
    (sfVector2f){view_bounds(c->render.view).left,
    view_bounds(c->render.view).top});
}

void init_game(core_t *c)
{
    c->render.window = create_window("SpaceGame");
    sfRenderWindow_setMouseCursorVisible(c->render.window, false);
    c->clock.clock = sfClock_create();
    sfTime prev_time = sfClock_getElapsedTime(c->clock.clock);
    init_textures(c);
    sfRenderWindow_setPosition(c->render.window, (sfVector2i){0, 0});
    c->mouse.last_pos.x = sfMouse_getPositionRenderWindow(c->render.window).x;
    c->mouse.last_pos.y = sfMouse_getPositionRenderWindow(c->render.window).y;
    c->mouse.diff = (sfVector2f){0, 0};
    c->render.view = sfView_create();
    c->entities.ships = NULL;
    c->entities.bolts = NULL;
    c->entities.explosions = NULL;
    c->entities.ships_nb = 0;
    c->skybox = NULL;
    c->render.cursor = sfSprite_create();
    sfSprite_setColor(c->render.cursor, sfWhite);
    sfSprite_setPosition(c->render.cursor, get_mouse_pos_view(c));
    sfSprite_setTexture(c->render.cursor, c->textures.crosshair, False);
    sfSprite_setScale(c->render.cursor, (sfVector2f){0.07, 0.07});
    sfSprite_setOrigin(c->render.cursor, get_sprite_center(c->render.cursor));
    init_player(c);
    init_view(c);
    randomise_spawns(c);
    init_sounds(c);
    init_fps_text(c);
    init_sky(c);
    init_hud(c);
    init_shaders(c);
}

void init_visualizer(visualizer_t vi)
{
    int sc;

    vi.song = sfSound_create();
    vi.sbuffer = sfSoundBuffer_createFromFile("sounds/music/song.ogg");
    sfSound_setBuffer(vi.song, vi.sbuffer);
    sc = sfSoundBuffer_getSampleCount(vi.sbuffer);
    vi.samples = malloc(sizeof(short) * (sc));

    for (int i = 0; i < sc; i++)
        vi.samples[i] = sfSoundBuffer_getSamples(vi.sbuffer)[i];
}