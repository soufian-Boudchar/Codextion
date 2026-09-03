/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/04 17:12:48 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:23:52 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

int	init_heap(t_data *data)
{
	data->heap.size = data->args.n_coders;
	data->heap.used = 0;
	data->heap.array = malloc(sizeof(t_request) * data->args.n_coders);
	pthread_mutex_init(&data->simulation.heap_mutex, NULL);
	if (!data->heap.array)
		return (1);
	return (0);
}

int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	data->dongles = malloc(sizeof(t_dongle) * data->args.n_coders);
	if (!data->dongles)
		return (1);
	while (i < data->args.n_coders)
	{
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		pthread_cond_init(&data->dongles[i].cond, NULL);
		data->dongles[i].available_at = -1;
		data->dongles[i].is_free = 1;
		i++;
	}
	return (0);
}

int	init_coders(t_data *data)
{
	int	i;

	i = 0;
	data->coders = malloc(sizeof(t_coder) * data->args.n_coders);
	if (!data->coders)
		return (1);
	while (i < data->args.n_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compiles_count = 0;
		data->coders[i].l_dongle = &data->dongles[i];
		data->coders[i].r_dongle = &data->dongles[(i + 1)
			% data->args.n_coders];
		data->coders[i].data = data;
		pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
		pthread_mutex_init(&data->coders[i].time_mutex, NULL);
		pthread_mutex_init(&data->coders[i].compiles_mutex, NULL);
		pthread_cond_init(&data->coders[i].coder_cond, NULL);
		i++;
	}
	return (0);
}

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->simulation.timer_mutex, NULL);
	pthread_mutex_init(&data->simulation.current_time_mutex, NULL);
	pthread_mutex_init(&data->monitor.game_over_mutex, NULL);
}
