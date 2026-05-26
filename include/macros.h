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

    #define EVENT_CLOSE 1
    #define EVENT_PAUSE 2
    #define IS_WALL 1

    #define PAUSE_RESUME 0
    #define PAUSE_MENU 1
    #define PAUSE_QUIT 2

    #define PBTN_OPT 0
    #define PBTN_SAVE 1
    #define PBTN_BACK 2
    #define PBTN_QUIT_ID 3
    #define PAUSE_BTN_COUNT 4

    #define PSCR_MAIN 0
    #define PSCR_OPT 1

    #define TILE_SIZE 64

    #define MAP_SAVE_PATH "./assets/maps/map_save.wolf"

    #define M_PI 3.14159265358979323846

    #define FOV (M_PI / 3)
    #define NUM_RAYS 800
    #define STEP 1.0
    #define DISTANCE_LIMIT 1.0f
    #define PLAYER_MARGIN 10.0f

    #define PLAYER_SPEED 2.0
    #define ROTATION_SPEED 0.05

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

    #define COL_TITLE sfColor_fromRGB(255, 140, 30)
    #define COL_HINT sfColor_fromRGB(170, 140, 100)
    #define COL_LABEL sfColor_fromRGB(195, 170, 130)
    #define COL_SEL sfColor_fromRGB(255, 175, 35)
    #define COL_BG_TOP sfColor_fromRGB(10, 8, 8)
    #define COL_BG_BOT sfColor_fromRGB(70, 15, 10)
    #define COL_BTN sfColor_fromRGBA(8, 4, 2, 195)
    #define COL_BTN_HOV sfColor_fromRGBA(28, 12, 4, 225)
    #define COL_BTN_BORDER sfColor_fromRGBA(180, 70, 15, 85)
    #define COL_BTN_BORDER_HOV sfColor_fromRGBA(255, 125, 25, 210)
    #define TITLE_FONT "assets/fonts/MetalMania.ttf"
    #define TITLE_BIG_SZ 80

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

    #define SND_MENU "assets/sounds/song_game-menu.wav"
    #define SND_GAME "assets/sounds/song-game.wav"
    #define SND_SHOOT "assets/sounds/spas12-sound.wav"

    #define FL_FEATHER 100.0f
    #define FL_R_BIG 1500.0f
    #define FL_N 64

    #define HEALTH_BAR_PATH "./assets/health_bar.png"
    #define HEALTH_FRAME_W 353
    #define HEALTH_FRAME_H 87
    #define HEALTH_COLS 2
    #define HEALTH_FRAMES 6

    #define AMMO_DEFAULT 30
    #define AMMO_FONT_SZ 30

    #define WALL_TEX_PATH "./assets/texture_wall_wolf.png"
    #define SKY_TEX_PATH "./assets/texture_sky.png"
    #define RELOAD_TEX_PATH "./assets/sprite_sheet_reload.png"
    #define RELOAD_COLS 3
    #define RELOAD_ROWS 2
    #define RELOAD_FRAME_COUNT 6
    #define RELOAD_FRAME_W 426
    #define RELOAD_FRAME_H 339
    #define RELOAD_FRAME_MS 120

typedef enum e_game_state {
    STATE_MENU,
    STATE_GAME,
    STATE_QUIT
} game_state_t;

typedef enum e_menu_screen {
    SCR_MAIN,
    SCR_MAP_SELECT,
    SCR_SETTINGS
} menu_screen_t;

typedef enum e_main_btn {
    BTN_PLAY = 0,
    BTN_SETTINGS,
    BTN_QUIT,
    MAIN_BTN_COUNT
} main_btn_t;

typedef enum e_map_btn {
    BTN_MAP_PLAY = 0,
    BTN_MAP_BACK,
    MAP_BTN_COUNT
} map_btn_t;

typedef enum e_set_btn {
    BTN_SET_BACK = 0,
    SET_BTN_COUNT
} set_btn_t;

#endif
