/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:17:04 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:24:00 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static void	signal_all_coders(t_data *data)
{
	int	c;

	c = 0;
	while (c < data->args.n_coders)
	{
		pthread_cond_signal(&data->coders[c].coder_cond);
		c++;
	}
}

static int	check_single_coder(t_data *data, int i, int *finished)
{
	struct timeval	current;
	long			burnout;

	gettimeofday(&current, NULL);
	pthread_mutex_lock(&data->coders[i].time_mutex);
	burnout = ((current.tv_sec - data->coders[i].lst_compile_tv.tv_sec) * 1000)
		+ ((current.tv_usec - data->coders[i].lst_compile_tv.tv_usec) / 1000);
	if (data->coders[i].compiles_count >= data->args.n_compiles_required)
		(*finished)++;
	pthread_mutex_unlock(&data->coders[i].time_mutex);
	if (burnout >= data->args.time_burnout)
	{
		get_elapsed_ms(data->simulation.start_time_tv, &burnout);
		pthread_mutex_lock(&data->monitor.game_over_mutex);
		data->monitor.game_over = 1;
		pthread_mutex_unlock(&data->monitor.game_over_mutex);
		signal_all_coders(data);
		printf("%ld %d burned out\n", burnout, data->coders[i].id);
		return (1);
	}
	return (0);
}

static int	check_status(t_data *data)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < data->args.n_coders)
	{
		if (check_single_coder(data, i, &finished))
			return (1);
		i++;
	}
	if (finished == data->args.n_coders)
	{
		pthread_mutex_lock(&data->monitor.game_over_mutex);
		data->monitor.game_over = 1;
		pthread_mutex_unlock(&data->monitor.game_over_mutex);
		signal_all_coders(data);
		return (1);
	}
	return (0);
}

static int	start_coders(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->simulation.timer_mutex, NULL);
	gettimeofday(&data->simulation.start_time_tv, NULL);
	while (i < data->args.n_coders)
	{
		pthread_mutex_lock(&data->coders[i].time_mutex);
		data->coders[i].lst_compile_tv = data->simulation.start_time_tv;
		pthread_mutex_unlock(&data->coders[i].time_mutex);
		if (pthread_create(&data->coders[i].tid, NULL, coder_routine,
				&data->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	monitor(t_data *data)
{
	if (start_coders(data) != 0)
		return (1);
	while (check_game_over(data) == 0)
	{
		if (check_status(data))
			break ;
		process_heap(data);
	}
	join_coders(data);
	return (0);
}
