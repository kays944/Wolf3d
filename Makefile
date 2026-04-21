##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Makefile
##

SRC_MAIN 	= 	src/mainc.c 		\

SRC			= 	src/main.c 					\
				src/wolf.c 					\
				src/close.c 				\
				src/draw.c 					\
				src/event.c 				\
				src/parsing_map.c 			\
				src/utils/free_array.c 		\
				src/utils/read_file.c 		\
				src/utils/str_split.c 		\

SRC_TESTS 	= 	$(filter-out $(SRC_MAIN), $(SRC))

OBJ_DIR		=	obj

OBJ			=	$(SRC:src/%.c=$(OBJ_DIR)/%.o)

CC 			:= 	epiclang

NAME		= 	wolf3d

CFLAGS 		=

CPPFLAGS	= 	-I./include

LDFLAGS 	= 	-lm -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio


all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/utils

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CPPFLAGS)

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
