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

    #define SAVE_PATH "./savegame.sav"

    #define M_PI 3.14159265358979323846

    #define FOV (M_PI / 3)
    #define NUM_RAYS 1920
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
    #define SET_DIFF 6
    #define SET_BACK 7
    #define SET_ITEM_COUNT 8
    #define DIFF_EASY 0
    #define DIFF_NORMAL 1
    #define DIFF_HARD 2
    #define DIFF_COUNT 3
    #define DIFF_DEFAULT DIFF_NORMAL
    #define SENS_DEFAULT 1.0f
    #define SENS_STEP 0.1f
    #define SENS_MIN 0.3f
    #define SENS_MAX 2.5f

    #define MENU_QUIT -1
    #define MENU_PLAY 0
    #define MENU_CONTINUE 2

    #define SCR_COUNT 3
    #define BITS_PER_PIXEL 32

    #define SND_MENU "assets/sounds/song_game-menu.wav"
    #define SND_SHOOT "assets/sounds/gun-fire.wav"
    #define SND_RELOAD "assets/sounds/gun-reload.wav"
    #define SND_STEP "assets/sounds/footstep.wav"
    #define SND_STEP2 "assets/sounds/footstep2.wav"
    #define SND_STEP3 "assets/sounds/footstep3.wav"
    #define SND_GROWL "assets/sounds/growl.wav"
    #define SND_BOSS "assets/sounds/boss-roar.wav"
    #define SND_HOWL "assets/sounds/howl.wav"
    #define SND_HURT "assets/sounds/hurt.wav"
    #define SND_PICKUP "assets/sounds/pickup.wav"
    #define SND_DOOR "assets/sounds/door-open.wav"
    #define SND_ESHOT "assets/sounds/fireball-shot.wav"
    #define SND_BITE "assets/sounds/bite.wav"
    #define SND_BOOM "assets/sounds/explosion.wav"
    #define SND_DIE_GRUNT "assets/sounds/die-grunt.wav"
    #define SND_DIE_RUNNER "assets/sounds/die-runner.wav"
    #define SND_DIE_BOSS "assets/sounds/die-boss.wav"
    #define SND_NIGHT1 "assets/sounds/night-breath.wav"
    #define SND_NIGHT2 "assets/sounds/night-weird.wav"
    #define SND_NIGHT3 "assets/sounds/night-creature.wav"
    #define SND_NIGHT_AMB "assets/sounds/night-ambience.wav"
    #define SND_DAY_AMB "assets/sounds/day-ambience.wav"

    #define FX_STEP 0
    #define FX_STEP2 1
    #define FX_STEP3 2
    #define FX_GROWL 3
    #define FX_BOSS 4
    #define FX_HOWL 5
    #define FX_HURT 6
    #define FX_PICKUP 7
    #define FX_DOOR 8
    #define FX_ESHOT 9
    #define FX_BITE 10
    #define FX_BOOM 11
    #define FX_DIE_GRUNT 12
    #define FX_DIE_RUNNER 13
    #define FX_DIE_BOSS 14
    #define FX_NIGHT1 15
    #define FX_NIGHT2 16
    #define FX_NIGHT3 17
    #define FX_KINDS 18
    #define NIGHT_FX_MIN 5.0f
    #define NIGHT_FX_VAR 7
    #define NIGHT_FX_DIST_MIN 250
    #define NIGHT_FX_DIST_VAR 350
    #define SND_POOL 10
    #define SND_MIN_DIST 110.0f
    #define SND_ATTENUATION 1.1f
    #define STEP_DIST 58.0f
    #define GROWL_CD 4.0f
    #define NIGHT_AMB_VOL 1.0f
    #define DAY_AMB_VOL 0.5f
    #define AMB_FADE_RATE 35.0f

    #define FL_FEATHER 100.0f
    #define FL_R_BIG 1500.0f
    #define FL_N 64
    #define FL_LIT_ON 0.52f
    #define FL_ALPHA_ON 235

    #define DAY_LEN 45.0f
    #define NIGHT_LEN 25.0f
    #define NIGHT_WARN_TIME 5.0f
    #define NIGHT_WAVE_CAP 12
    #define NIGHT_MSG_TIME 2.0f
    #define NIGHT_LIT_OFF 0.10f
    #define NIGHT_ALPHA_OFF 252
    #define NIGHT_SIGHT 260.0f
    #define FL_SIGHT_MULT 3.0f
    #define NIGHT_DARK_FLOOR 0.05f
    #define NIGHT_FONT_SZ 44
    #define COL_NIGHT sfColor_fromRGB(150, 60, 220)
    #define COL_NIGHTFALL sfColor_fromRGB(235, 70, 45)
    #define COL_DAWN sfColor_fromRGB(250, 200, 90)

    #define HEALTH_BAR_PATH "./assets/health_bar.png"
    #define HEALTH_FRAME_W 353
    #define HEALTH_FRAME_H 87
    #define HEALTH_COLS 2
    #define HEALTH_FRAMES 6

    #define MAX_DOORS 24
    #define DOOR_OPEN_DIST (TILE_SIZE * 1.35f)
    #define DOOR_HINT_SZ 26
    #define DOOR_HINT_Y 0.60f
    #define KEY_TEX_PATH "./assets/key.png"
    #define KEY_PICK_DIST (TILE_SIZE * 0.75f)
    #define EXIT_DIST (TILE_SIZE * 0.7f)
    #define MARK_WORLD_SIZE 30.0f
    #define COL_KEY sfColor_fromRGB(255, 210, 40)
    #define COL_EXIT sfColor_fromRGB(60, 230, 120)

    #define MINI_CELL 7.0f
    #define MINI_MARGIN 14.0f
    #define MINI_MAX_W 0.26f
    #define MINI_TOP_OFFSET 88.0f
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

    #define FPS_FONT_SZ 22
    #define FPS_REFRESH 0.5f

    #define SCORE_ENEMY 100
    #define SCORE_BOSS 500
    #define SCORE_HEADSHOT 50
    #define SCORE_FONT_SZ 26

    #define AMMO_DEFAULT 30
    #define AMMO_FONT_SZ 30
    #define AMMO_RESERVE_START 30
    #define AMMO_RESERVE_MAX 60
    #define AMMO_BOX_VALUE 12
    #define AMMO_TEX_PATH "./assets/props/ammo_box.png"

    #define WALL_TEX_PATH "./assets/texture_wall_wolf.png"
    #define BRICK_TEX_PATH "./assets/texture_wall_brick.png"
    #define COLD_TEX_PATH "./assets/texture_wall_cold.png"
    #define DOOR_TEX_PATH "./assets/texture_door.png"
    #define DOOR_LOCKED_TEX_PATH "./assets/texture_door_locked.png"
    #define WALL_KINDS 5
    #define WALL_STONE 0
    #define WALL_BRICK 1
    #define WALL_COLD 2
    #define WALL_DOOR 3
    #define WALL_DOOR_LOCKED 4
    #define SKY_TEX_PATH "./assets/texture_sky.png"
    #define SKY_NIGHT_TEX_PATH "./assets/texture_sky_night.png"
    #define FLOOR_TEX_PATH "./assets/texture_floor.png"
    #define RELOAD_TIME 1.40f
    #define RELOAD_DIP_T 0.30f
    #define RELOAD_DIP_FRAC 0.85f
    #define RELOAD_TILT 10.0f

    #define ENEMY_TEX_PATH "./assets/enemy.png"
    #define BOSS_TEX_PATH "./assets/boss.png"
    #define RUNNER_TEX_PATH "./assets/runner.png"
    #define MAX_ENEMIES 32
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
    #define RUNNER_STOP_DIST 30.0f
    #define RUNNER_MELEE_RANGE 52.0f
    #define RUNNER_MELEE_DMG 16
    #define RUNNER_MELEE_CD 1.0f
    #define BOSS_SCALE 1.6f
    #define ENEMY_SPEED 95.0f
    #define ENEMY_SIGHT 950.0f
    #define ENEMY_STOP_DIST 170.0f
    #define ENEMY_SHOOT_RANGE 800.0f
    #define ENEMY_SHOOT_CD 1.8f
    #define ENEMY_FIRST_CD_MIN 1.0f
    #define ENEMY_DMG 14
    #define BOSS_DMG 32

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
    #define WEAPON_X_RATIO 0.055f
    #define AIM_ZOOM 1.10f
    #define AIM_SENS_MULT 0.45f
    #define AIM_HEAD_MULT 1.35f

    #define PM_NONE 0
    #define PM_UP 1
    #define PM_DOWN 2
    #define PM_LEFT 3
    #define PM_RIGHT 4
    #define PM_OK 5
    #define PM_BACK 6
    #define PAD_BTN_OK 0
    #define PAD_BTN_MENU_BACK 2
    #define PAD_MENU_HI 60.0f
    #define PAD_MENU_LO 40.0f

    #define PAD_ID 0
    #define PAD_DEADZONE 18.0f
    #define PAD_LOOK_SPEED 2.4f
    #define PAD_PITCH_SPEED 430.0f
    #define PAD_BTN_JUMP 0
    #define PAD_BTN_FLASH 2
    #define PAD_BTN_RELOAD 3
    #define PAD_BTN_FIRE 7
    #define PAD_BTN_PAUSE 9
    #define PAD_BTN_PICKUP 1
    #define PAD_AIM_AXIS sfJoystickZ
    #define PAD_FIRE_AXIS sfJoystickR
    #define TRIG_ON 40.0f
    #define TRIG_OFF 5.0f
    #define ENEMY_RADIUS 24.0f
    #define HEAD_RADIUS 9.0f
    #define LOS_STEP 4.0f

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
    #define PACK_HP 20
    #define PACK_RADIUS 48.0f
    #define MAX_POPUPS 8
    #define POPUP_LIFE 0.9f
    #define POPUP_RISE 70.0f
    #define POPUP_FONT 32
    #define COL_POP_AMMO sfColor_fromRGB(255, 210, 60)
    #define COL_POP_HP sfColor_fromRGB(90, 230, 120)
    #define POP_DMG 2
    #define POP_HEAD 3
    #define COL_POP_DMG sfColor_fromRGB(240, 240, 240)
    #define COL_POP_HEAD sfColor_fromRGB(255, 80, 40)
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

    #define BOOM_TEX_PATH "./assets/explosion.png"
    #define MAX_BOOMS 16
    #define BOOM_FRAMES 6
    #define BOOM_FRAME_W 96
    #define BOOM_FRAME_H 96
    #define BOOM_FRAME_LEN 0.09f
    #define BOOM_WORLD_SIZE 130.0f
    #define BOOM_RADIUS 150.0f
    #define BOOM_DMG 120
    #define BOOM_PLAYER_DMG 40
    #define BOOM_CHAIN_MIN 0.15f
    #define BOOM_CHAIN_VAR 0.20f
    #define BOOM_SND_DIST 450.0f
    #define BARREL_RADIUS 20.0f

    #define SHOT_RANGE 700.0f
    #define SHOT_DMG 30
    #define HEADSHOT_DMG 65
    #define HPBAR_W_RATIO 0.7f
    #define HPBAR_H_RATIO 0.045f
    #define HPBAR_MIN_H 3.0f
    #define HPBAR_GAP 5.0f
    #define PLAYER_HP 100
    #define HURT_COOLDOWN 0.6f
    #define HURT_FLASH_FRAMES 12
    #define HP_FONT_SZ 30
    #define CROSS_SIZE 16.0f
    #define CROSS_SIZE_AIM 9.0f
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
    BTN_CONTINUE,
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
