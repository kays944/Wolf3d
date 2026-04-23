/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** wolf.h
*/

#ifndef WOLF_H_
    #define WOLF_H_
    #include <SFML/Window.h>
    #include <SFML/Graphics.h>
    #include <SFML/System.h>
    #include <SFML/System/Vector2.h>

typedef struct player_s {
    float x;
    float y;
    float angle;
} player_t;


int wolf(void);
void draw(sfRenderWindow *window, player_t *player, char **map);
int event(sfRenderWindow *window, player_t *player, char **map);
void close_all(sfRenderWindow *window);
char **parsing_map(char *path);
void update_player(sfRenderWindow *window, player_t *player, char **map);

int is_wall(int x, int y, char **map);

#endif
