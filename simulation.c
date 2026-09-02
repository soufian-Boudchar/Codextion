#include "codexion.h"



void	update_simulation_current_time(t_data *data, long *current_time_ms)
{
	pthread_mutex_lock(&data->simulation.timer_mutex);
	get_elapsed_ms(data->simulation.start_time_tv, current_time_ms);
	pthread_mutex_unlock(&data->simulation.timer_mutex);
}

int	check_game_over(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->monitor.game_over_mutex);
	status = data->monitor.game_over;
	pthread_mutex_unlock(&data->monitor.game_over_mutex);
	return (status);
}

void	print_action(t_coder *coder, char *action)
{
	long	time;

	pthread_mutex_lock(&coder->data->monitor.game_over_mutex);
	if (coder->data->monitor.game_over == 0)
	{
		get_elapsed_ms(coder->data->simulation.start_time_tv, &time);
		printf("%ld %d %s\n", time, coder->id, action);
	}
	pthread_mutex_unlock(&coder->data->monitor.game_over_mutex);
}

void	*coder_routine(void *coder_ptr)
{
	t_coder *coder;
	t_request req;
	long current_time_ms;
	coder = (t_coder *)coder_ptr;
	req.coder_id = coder->id;
	long now;

	while (check_game_over(coder->data) == 0)
	{
		if (coder->data->args.sch == 0)
		{ // if scheduler == FIFO
			get_elapsed_ms(coder->data->simulation.start_time_tv,
				&current_time_ms);
			req.priority = current_time_ms;
		}
		else if (coder->data->args.sch == 1)
		{ // if scheduler == EDF
			long last_compile_from_start;

			pthread_mutex_lock(&coder->time_mutex);
			// N-7sbo ch7al dyal l-wa9t daz mn start_time_tv 7ta akher compile
			last_compile_from_start = ((coder->lst_compile_tv.tv_sec
						- coder->data->simulation.start_time_tv.tv_sec) * 1000)
				+ ((coder->lst_compile_tv.tv_usec
						- coder->data->simulation.start_time_tv.tv_usec)
					/ 1000);
			pthread_mutex_unlock(&coder->time_mutex);

			// Priority hiya Deadline dyal L-Mawt b d-dabt (Absolute Time)
			req.priority = last_compile_from_start
				+ coder->data->args.time_burnout;
		}

		pthread_mutex_lock(&coder->data->simulation.heap_mutex);

		pthread_mutex_lock(&coder->coder_mutex);
		heap_push(&coder->data->heap, req);
		pthread_mutex_unlock(&coder->data->simulation.heap_mutex);

		pthread_cond_wait(&coder->coder_cond, &coder->coder_mutex);
		pthread_mutex_unlock(&coder->coder_mutex);

		if (check_game_over(coder->data) != 0)
			return (coder_ptr);

		pthread_mutex_lock(&coder->l_dongle->mutex);
		print_action(coder, "has taken a dongle");

		pthread_mutex_lock(&coder->r_dongle->mutex);
		print_action(coder, "has taken a dongle");

		pthread_mutex_lock(&coder->time_mutex);
		gettimeofday(&coder->lst_compile_tv, NULL);
		coder->compiles_count++;
		pthread_mutex_unlock(&coder->time_mutex);

		print_action(coder, "is compiling");
		usleep(coder->data->args.time_compile * 1000);
		pthread_mutex_lock(&coder->data->simulation.heap_mutex);
		get_elapsed_ms(coder->data->simulation.start_time_tv, &now);
		coder->l_dongle->available_at = now + coder->data->args.dongle_cooldown;
		coder->r_dongle->available_at = now + coder->data->args.dongle_cooldown;

		coder->l_dongle->is_free = 1;
		coder->r_dongle->is_free = 1;
		pthread_mutex_unlock(&coder->data->simulation.heap_mutex);

		pthread_mutex_unlock(&coder->l_dongle->mutex);
		pthread_mutex_unlock(&coder->r_dongle->mutex);

		print_action(coder, "is debugging");

		usleep(coder->data->args.time_debug * 1000);
		print_action(coder, "is refactoring");

		usleep(coder->data->args.time_refactor * 1000);
	}
	return (coder_ptr);
}