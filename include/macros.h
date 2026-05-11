/*
** EPITECH PROJECT, 2025
** wolf3d
** File description:
** macros.h
*/

#ifndef MACRO_H_
    #define MACRO_H_

    #define EXIT_SUCCESS 0
    #define EXIT_FAIL 84

    #define WIN_WIDTH 1920
    #define WIN_HEIGHT 1080
    #define FREQUENCY 32

    #define EVENT_CLOSE 1
    #define IS_WALL 1

    #define TILE_SIZE 64
    #define MAP_WIDTH 8
    #define MAP_HEIGHT 8

    #define BASIC_MAP_PATH "./assets/basic_map.txt"

    #define M_PI 3.14159265358979323846

    #define FOV (M_PI / 3)
    #define NUM_RAYS 800
    #define STEP 1.0
    #define DISTANCE_LIMIT 0.00001f

    #define PLAYER_SPEED 2.0
    #define ROTATION_SPEED 0.05

extern int map[MAP_HEIGHT][MAP_WIDTH];

    #define WIN_W 1280
    #define WIN_H 720
    #define TITLE "Wolf3D"
    #define FPS_LIMIT 60

    #define BTN_W 320
    #define BTN_H 58
    #define BTN_GAP 18

    #define TITLE_SZ 50
    #define FONT_BTN_SZ 24
    #define FONT_SMALL_SZ 18
    #define FONT_LABEL_SZ 20

    #define COL_TITLE sfColor_fromRGB(220, 50, 30)
    #define COL_HINT sfColor_fromRGB(150, 150, 150)
    #define COL_LABEL sfColor_fromRGB(200, 200, 200)
    #define COL_SEL sfColor_fromRGB(255, 200, 40)
    #define COL_BG_TOP sfColor_fromRGB(10, 8, 8)
    #define COL_BG_BOT sfColor_fromRGB(70, 15, 10)
    #define COL_BTN sfColor_fromRGBA(35, 35, 35, 220)
    #define COL_BTN_HOV sfColor_fromRGBA(110, 25, 15, 240)

    #define MAX_MAPS 16
    #define MAP_NAME_LEN 64
    #define MAP_DIR "assets/maps"
    #define MAP_EXT ".wolf"
    #define CFG_PATH "settings.cfg"

    #define NUM_RES 4
    #define VOL_DEFAULT 80.0f
    #define VOL_STEP 5.0f
    #define VOL_MIN 0.0f
    #define VOL_MAX 100.0f
    #define RES_DEFAULT 2

    #define SET_MUSIC 0
    #define SET_SFX 1
    #define SET_RES 2
    #define SET_FULLSCR 3
    #define SET_BACK 4
    #define SET_ITEM_COUNT 5

    #define MENU_QUIT -1
    #define MENU_PLAY 0

    #define SCR_COUNT 3
    #define BITS_PER_PIXEL 32


#endif
