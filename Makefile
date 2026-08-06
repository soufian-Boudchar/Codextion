run:
	@cc -g -Wall -Wextra -pthread main.c utils.c init.c heap.c validator.c codexion.h -o ./codexion
	./codexion 5 100 100 100 50 20 50 fifo
format:
	@c_formatter_42 *.c *.h
