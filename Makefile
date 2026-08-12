run:
	@cc -g -Wall -Wextra -pthread main.c utils.c init.c heap.c simulation.c validator.c codexion.h -o ./codexion
	./codexion 10 3000 200 100 200 10 400 fifo
format:
	@c_formatter_42 *.c *.h
