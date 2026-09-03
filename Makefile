run:
	@cc -g -Wall -Wextra -pthread main.c simulation_utils.c monitor_heap.c monitor.c utils.c init.c heap.c simulation.c validator.c codexion.h -o ./codexion
	./codexion 5 3000 200 100 200 5 400 edf
format:
	@c_formatter_42 *.c *.h
leak:
	gcc -g -fsanitize=address -fno-omit-frame-pointer main.c utils.c init.c heap.c simulation.c validator.c free.c codexion.h -o ./leak
