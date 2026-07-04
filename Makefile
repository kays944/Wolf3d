##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Makefile
##

SRC_MAIN 	= 	src/mainc.c 						\

SRC			= 	src/main.c 							\
				src/wolf.c 							\
				src/close.c 						\
				src/draw.c 							\
				src/event.c 						\
				src/parsing_map.c 					\
				src/update_player.c 				\
				src/sound.c 						\
				src/game.c 							\
				src/pause.c 						\
				src/pause_render.c 					\
				src/save.c 							\
				src/reload.c 						\
				src/wall_tex.c 						\
				src/texture_floor_ceil.c 			\
				src/hud_fx.c 						\
				src/enemies/enemy.c 				\
				src/enemies/enemy_utils.c 			\
				src/enemies/enemy_ai.c 				\
				src/enemies/enemy_draw.c 			\
				src/enemies/enemy_shoot.c 			\
				src/tools/weapon.c 					\
				src/tools/flashlight.c 				\
				src/tools/ammo.c 					\
				src/tools/load_health.c 			\
				src/utils/free_array.c 				\
				src/utils/read_file.c 				\
				src/utils/str_split.c 				\
				src/game-menu/button.c 				\
				src/game-menu/map_select.c 			\
				src/game-menu/menu_events.c 		\
				src/game-menu/menu_render.c 		\
				src/game-menu/menu.c 				\
				src/game-menu/settings_render.c 	\
				src/game-menu/settings.c 			\
				src/game-menu/init.c 				\


SRC_TESTS 	= 	$(filter-out $(SRC_MAIN), $(SRC))

OBJ_DIR		=	obj

OBJ			=	$(SRC:src/%.c=$(OBJ_DIR)/%.o)

CC 			:= 	gcc

NAME		= 	wolf3d

CFLAGS 		= 	-O2

CPPFLAGS	= 	-I./include

LDFLAGS 	= 	-lm -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio


all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/utils
	mkdir -p $(OBJ_DIR)/game-menu
	mkdir -p $(OBJ_DIR)/tools
	mkdir -p $(OBJ_DIR)/enemies

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) -r $(OBJ_DIR)

fclean:		clean
	$(RM) $(NAME)

re:	fclean all

debug: CFLAGS += -g
debug:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(CPPFLAGS)


.PHONY: all clean fclean re debug
