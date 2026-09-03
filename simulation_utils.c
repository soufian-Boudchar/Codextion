#include "codexion.h"

void	update_simulation_current_time(t_data *data, long *current_time_ms)
{
	pthread_mutex_lock(&data->simulation.timer_mutex);
	get_elapsed_ms(data->simulation.start_time_tv, current_time_ms);
	pthread_mutex_unlock(&data->simulation.timer_mutex);
}

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->l_dongle->mutex);
	print_action(coder, "has taken a dongle");
	pthread_mutex_lock(&coder->r_dongle->mutex);
	print_action(coder, "has taken a dongle");
}

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->time_mutex);
	gettimeofday(&coder->lst_compile_tv, NULL);
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->time_mutex);
	print_action(coder, "is compiling");
	usleep(coder->data->args.time_compile * 1000);
}

void	release_dongles(t_coder *coder)
{
	long	now;

	pthread_mutex_lock(&coder->data->simulation.heap_mutex);
	get_elapsed_ms(coder->data->simulation.start_time_tv, &now);
	coder->l_dongle->available_at = now + coder->data->args.dongle_cooldown;
	coder->r_dongle->available_at = now + coder->data->args.dongle_cooldown;
	coder->l_dongle->is_free = 1;
	coder->r_dongle->is_free = 1;
	pthread_mutex_unlock(&coder->data->simulation.heap_mutex);
	pthread_mutex_unlock(&coder->l_dongle->mutex);
	pthread_mutex_unlock(&coder->r_dongle->mutex);
}

void	debug_refactor(t_coder *coder)
{
	print_action(coder, "is debugging");
	usleep(coder->data->args.time_debug * 1000);
	print_action(coder, "is refactoring");
	usleep(coder->data->args.time_debug * 1000);
}
