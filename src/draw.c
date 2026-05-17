/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** creation.c
*/

#include "wolf.h"
#include "macros.h"
#include <math.h>
#include <stdio.h>

/*
**  Fonction de detection si les coordonnees en parametre
**  correspondent a un mur sur la map
*/
int is_wall(int x, int y, char **map)
{
    int tile_x = x / TILE_SIZE;
    int tile_y = y / TILE_SIZE;

    if (tile_x < 0
        || tile_x >= MAP_WIDTH
        || tile_y < 0
        || tile_y >= MAP_HEIGHT
        || map[tile_y][tile_x] == 'x')
        return IS_WALL;
    return EXIT_SUCCESS;
}

/*
**  Fonction pour dessiner le plafond et le sol en divisant l'ecran
**  par deux et en dessinant un rectangle en bas et un autre en haut
*/
static void draw_floor_and_ceiling(sfRenderWindow *window)
{
    sfRectangleShape *rect = sfRectangleShape_create();

    sfRectangleShape_setSize(rect, (sfVector2f){WIN_WIDTH, WIN_HEIGHT / 2});
    sfRectangleShape_setPosition(rect, (sfVector2f){0, 0});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(50, 50, 50));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_setPosition(rect, (sfVector2f){0, WIN_HEIGHT / 2});
    sfRectangleShape_setFillColor(rect, sfColor_fromRGB(100, 100, 100));
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
    sfRectangleShape_destroy(rect);
}

/*
**  Calcul de la hauteur de la colonne
*/
static void render_wall_column(sfRenderWindow *window, sfRectangleShape *rect,
    int rays, float wall_height)
{
    float col_width = (float)WIN_WIDTH / NUM_RAYS;

    sfRectangleShape_setSize(rect, (sfVector2f){col_width, wall_height});
    sfRectangleShape_setPosition(rect, (sfVector2f){
            rays * col_width,
            WIN_HEIGHT / 2 - wall_height / 2
    });
    sfRectangleShape_setFillColor(rect, sfBlack);
    sfRenderWindow_drawRectangleShape(window, rect, NULL);
}

/*
**  Calcule la distance du mur devant soi avec un pas de 0.1
*/
static float cast_single_ray(player_t *player, float ray_angle, char **map)
{
    float distance = 0;
    float x = player->x;
    float y = player->y;

    while (is_wall(x, y, map) != IS_WALL) {
        x += cos(ray_angle) * STEP;
        y += sin(ray_angle) * STEP;
    }
    distance = sqrt((x - player->x) * (x - player->x) + (y - player->y) *
        (y - player->y)) * cos(player->angle - ray_angle);
    return distance;
}

/*
**  Boucle sur les 800 rayons, calcul l'angle, la distance (cast_single_ray),
**  verfie et calcule la hauteur et affiche (render_wall_column)
*/
static void cast_all_rays(sfRenderWindow *window, player_t *player, char **map)
{
    sfRectangleShape *rect = sfRectangleShape_create();
    float ray_angle = 0;
    float distance = 0;
    float wall_height = 0;

    for (size_t rays = 0; rays < NUM_RAYS; ++rays) {
        ray_angle = player->angle - (FOV / 2) + (FOV * rays / NUM_RAYS);
        ray_angle = fmod(ray_angle + 2 * M_PI, 2 * M_PI);
        distance = cast_single_ray(player, ray_angle, map);
        if (distance < DISTANCE_LIMIT)
            distance = DISTANCE_LIMIT;
        wall_height = (TILE_SIZE * WIN_HEIGHT) / distance;
        if (wall_height > WIN_HEIGHT)
            wall_height = WIN_HEIGHT;
        render_wall_column(window, rect, rays, wall_height);
    }
    sfRectangleShape_destroy(rect);
}

/*
**  Fonction qui clear l'espace de dessin et qui appelle les fonctions
**  qui dessine les elements un par un, et qui finit par les afficher
*/
void draw(sfRenderWindow *window, player_t *player, char **map)
{
    sfRenderWindow_clear(window, sfBlack);
    draw_floor_and_ceiling(window);
    cast_all_rays(window, player, map);
    if (player->weapon_spr)
        sfRenderWindow_drawSprite(window, player->weapon_spr, NULL);
    sfRenderWindow_display(window);
}
