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

    #define PLAYER_SPEED 175.0f
    #define ROTATION_SPEED 3.0f
    #define DT_MAX 0.05f

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
    #define SET_INPUT 4
    #define SET_SENS 5
    #define SET_BACK 6
    #define SET_ITEM_COUNT 7
    #define SENS_DEFAULT 1.0f
    #define SENS_STEP 0.1f
    #define SENS_MIN 0.3f
    #define SENS_MAX 2.5f

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
    #define FL_LIT_ON 0.52f
    #define FL_LIT_OFF 0.17f
    #define FL_ALPHA_ON 140
    #define FL_ALPHA_OFF 232
    #define FL_BEAM_HALF 0.40f
    #define FL_BEAM_RANGE 680.0f
    #define FL_BLIND_TIME 0.5f
    #define FL_BLIND_SLOW 0.35f

    #define HEALTH_BAR_PATH "./assets/health_bar.png"
    #define HEALTH_FRAME_W 353
    #define HEALTH_FRAME_H 87
    #define HEALTH_COLS 2
    #define HEALTH_FRAMES 6

    #define MAX_DOORS 24
    #define DOOR_OPEN_DIST (TILE_SIZE * 1.35f)
    #define KEY_PICK_DIST (TILE_SIZE * 0.75f)
    #define EXIT_DIST (TILE_SIZE * 0.7f)
    #define MARK_WORLD_SIZE 30.0f
    #define COL_KEY sfColor_fromRGB(255, 210, 40)
    #define COL_EXIT sfColor_fromRGB(60, 230, 120)

    #define MINI_CELL 7.0f
    #define MINI_MARGIN 14.0f
    #define MINI_DOT 2.4f
    #define MINI_PLAYER_DOT 3.4f
    #define MINI_BOSS_DOT 4.2f
    #define MINI_DIR_LEN 9.0f
    #define COL_MINI_BG sfColor_fromRGBA(0, 0, 0, 150)
    #define COL_MINI_WALL sfColor_fromRGBA(205, 120, 45, 205)
    #define COL_MINI_PLAYER sfColor_fromRGB(90, 225, 100)
    #define COL_MINI_GRUNT sfColor_fromRGB(235, 70, 45)
    #define COL_MINI_BRUTE sfColor_fromRGB(255, 140, 140)
    #define COL_MINI_RUNNER sfColor_fromRGB(150, 230, 255)
    #define COL_MINI_BOSS sfColor_fromRGB(255, 160, 30)

    #define SCORE_ENEMY 100
    #define SCORE_BOSS 500
    #define SCORE_HEADSHOT 50
    #define SCORE_FONT_SZ 26

    #define AMMO_DEFAULT 30
    #define AMMO_FONT_SZ 30
    #define AMMO_RESERVE_START 60
    #define AMMO_RESERVE_MAX 90
    #define AMMO_BOX_VALUE 20
    #define AMMO_TEX_PATH "./assets/props/ammo_box.png"

    #define WALL_TEX_PATH "./assets/texture_wall_wolf.png"
    #define SKY_TEX_PATH "./assets/texture_sky.png"
    #define RELOAD_TEX_PATH "./assets/sprite_sheet_reload.png"
    #define RELOAD_COLS 3
    #define RELOAD_ROWS 2
    #define RELOAD_FRAME_COUNT 6
    #define RELOAD_FRAME_W 426
    #define RELOAD_FRAME_H 339
    #define RELOAD_FRAME_MS 120

    #define ENEMY_TEX_PATH "./assets/enemy.png"
    #define BOSS_TEX_PATH "./assets/boss.png"
    #define MAX_ENEMIES 16
    #define ENEMY_HP 100
    #define ENEMY_HP_PER_LEVEL 25
    #define BOSS_HP (ENEMY_HP * 2)
    #define ENEMY_TYPE_GRUNT 0
    #define ENEMY_TYPE_BRUTE 1
    #define ENEMY_TYPE_RUNNER 2
    #define ENEMY_TYPE_BOSS 3
    #define BRUTE_HP (ENEMY_HP * 2)
    #define BRUTE_SPEED 55.0f
    #define RUNNER_HP 60
    #define RUNNER_SPEED 155.0f
    #define BOSS_SCALE 1.6f
    #define ENEMY_SPEED 95.0f
    #define ENEMY_SIGHT 700.0f
    #define ENEMY_STOP_DIST 170.0f
    #define ENEMY_SHOOT_RANGE 550.0f
    #define ENEMY_SHOOT_CD 1.8f
    #define ENEMY_FIRST_CD_MIN 1.0f
    #define ENEMY_DMG 10
    #define BOSS_DMG 25

    #define PROJ_TEX_PATH "./assets/fireball.png"
    #define MAX_PROJS 64
    #define PROJ_SPEED 320.0f
    #define PROJ_HIT_RADIUS 26.0f
    #define PROJ_WORLD_SIZE 24.0f
    #define PROJ_DODGE_Z 12.0f

    #define JUMP_VEL 170.0f
    #define GRAVITY 800.0f
    #define PITCH_SPEED 550.0f
    #define PITCH_MAX_DIV 3
    #define WEAPON_X_RATIO 0.034f

    #define PAD_ID 0
    #define PAD_DEADZONE 18.0f
    #define PAD_LOOK_SPEED 2.4f
    #define PAD_PITCH_SPEED 430.0f
    #define PAD_BTN_JUMP 0
    #define PAD_BTN_FLASH 2
    #define PAD_BTN_RELOAD 3
    #define PAD_BTN_FIRE 7
    #define PAD_BTN_PAUSE 9
    #define ENEMY_RADIUS 24.0f
    #define HEAD_RADIUS 9.0f
    #define LOS_STEP 4.0f
    #define ENEMY_SND_PITCH 0.6f
    #define ENEMY_SND_VOL 0.5f

    #define ANIM_COLS 5
    #define ANIM_ROWS 3
    #define WALK_FRAMES 4
    #define ATK_FRAMES 3
    #define WALK_FPS 7.0f
    #define ATK_ANIM_LEN 0.45f
    #define DEATH_ROW 2
    #define DEATH_FRAMES 5
    #define DEATH_FRAME_LEN 0.12f

    #define PACK_TEX_PATH "./assets/medkit.png"
    #define MAX_PACKS 16
    #define PACK_HP 25
    #define PACK_RADIUS 30.0f
    #define PACK_WORLD_SIZE 22.0f
    #define PACK_KINDS 2
    #define PACK_MEDKIT 0
    #define PACK_AMMO 1

    #define MAX_PROPS 32
    #define PROP_TYPES 4
    #define PROP_BARREL 0
    #define PROP_CANDLE 1
    #define PROP_SKELETON 2
    #define PROP_GORE 3

    #define SHOT_RANGE 700.0f
    #define SHOT_DMG 50
    #define HEADSHOT_DMG 100
    #define HPBAR_W_RATIO 0.7f
    #define HPBAR_H_RATIO 0.045f
    #define HPBAR_MIN_H 3.0f
    #define HPBAR_GAP 5.0f
    #define PLAYER_HP 100
    #define HURT_COOLDOWN 0.6f
    #define HURT_FLASH_FRAMES 12
    #define HP_FONT_SZ 30
    #define CROSS_SIZE 16.0f
    #define CROSS_THICK 3.0f

    #define END_MSG_WIN "VICTOIRE !"
    #define END_MSG_LOSE "T'ES MORT"
    #define END_FONT_SZ 80
    #define END_NEXT 10
    #define END_RESTART 11
    #define END_MODE_LOSE 0
    #define END_MODE_WIN_NEXT 1
    #define END_MODE_WIN_LAST 2
    #define END_BG_TOP sfColor_fromRGB(8, 6, 6)
    #define END_BG_WIN sfColor_fromRGB(95, 58, 14)
    #define END_BG_LOSE sfColor_fromRGB(88, 12, 10)
    #define END_BAR_W 340.0f
    #define END_BAR_H 4.0f
    #define END_TITLE_Y 0.22f
    #define END_BAR_Y 0.33f
    #define END_BTN_Y 0.44f

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
