#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int	main(void)
{
	int i = 0;
	while (i++ < 1000)
		printf("pthread_create: %s\n", strerror(i));
}