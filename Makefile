NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
OBJ_DIR = object_files

SRC = main.c simulation_utils.c \
	  monitor_heap.c monitor.c \
	  utils.c init.c heap.c \
	  simulation.c validator.c
OBJ = $(SRC:.c=.o)
HEADER = ./codexion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re