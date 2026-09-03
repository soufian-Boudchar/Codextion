NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

OBJ_DIR = obj

SRC = src/main.c src/simulation_utils.c \
      src/monitor_heap.c src/monitor.c \
      src/utils.c src/init.c src/heap.c \
      src/simulation.c src/validator.c

OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)

HEADER = header/codexion.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c $(HEADER)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re