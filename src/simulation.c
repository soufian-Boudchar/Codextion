/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 02:31:48 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:24:07 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

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

static long	get_priority(t_coder *coder)
{
	long	priority;
	long	current_time_ms;
	t_data	*data;
	long	last_compile_from_start;

	data = coder->data;
	if (data->args.sch == 0)
	{
		get_elapsed_ms(data->simulation.start_time_tv, &current_time_ms);
		priority = current_time_ms;
	}
	else if (data->args.sch == 1)
	{
		pthread_mutex_lock(&coder->time_mutex);
		last_compile_from_start = ((coder->lst_compile_tv.tv_sec
					- data->simulation.start_time_tv.tv_sec) * 1000)
			+ ((coder->lst_compile_tv.tv_usec
					- data->simulation.start_time_tv.tv_usec) / 1000);
		pthread_mutex_unlock(&coder->time_mutex);
		priority = last_compile_from_start + data->args.time_burnout;
	}
	return (priority);
}

void	*coder_routine(void *coder_ptr)
{
	t_coder		*coder;
	t_request	req;
	t_data		*data;

	coder = (t_coder *)coder_ptr;
	data = coder->data;
	req.coder_id = coder->id;
	while (check_game_over(coder->data) == 0)
	{
		req.priority = get_priority(coder);
		pthread_mutex_lock(&data->simulation.heap_mutex);
		pthread_mutex_lock(&coder->coder_mutex);
		heap_push(&data->heap, req);
		pthread_mutex_unlock(&data->simulation.heap_mutex);
		pthread_cond_wait(&coder->coder_cond, &coder->coder_mutex);
		pthread_mutex_unlock(&coder->coder_mutex);
		if (check_game_over(coder->data) != 0)
			return (coder_ptr);
		take_dongles(coder);
		compile(coder);
		release_dongles(coder);
		debug_refactor(coder);
	}
	return (coder_ptr);
}
