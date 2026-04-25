##
## EPITECH PROJECT, 2025
## Makefile
## File description:
## Makefile
##

SRC_MAIN 	= 	src/main.c 			\

SRC			=	$(shell find src/ -name "*.c")

SRC_TESTS 	= 	$(filter-out $(SRC_MAIN), $(SRC))

OBJ_DIR		=	obj

OBJ			=	$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

CC 			:= 	epiclang

NAME		= 	wolf3d

CFLAGS 		=

CPPFLAGS	= 	-I./include

LDFLAGS 	= 	-lm -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio


all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CPPFLAGS)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

clean:
	$(RM) -f $(OBJ)

fclean:		clean
	$(RM) -f $(NAME)

re:	fclean all

debug: CFLAGS += -g
debug:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(CPPFLAGS)


.PHONY: all clean fclean re debug
