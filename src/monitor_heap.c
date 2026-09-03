/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_heap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 05:17:44 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:23:58 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static int	can_dispatch(t_data *data, int top_id, long now, int *needed)
{
	int	l_id;
	int	r_id;

	l_id = top_id;
	r_id = (top_id + 1) % data->args.n_coders;
	if (data->coders[top_id].l_dongle->is_free
		&& data->coders[top_id].r_dongle->is_free
		&& now >= data->coders[top_id].l_dongle->available_at
		&& now >= data->coders[top_id].r_dongle->available_at
		&& needed[l_id] == 0 && needed[r_id] == 0)
		return (1);
	return (0);
}

static void	dispatch_coder(t_data *data, int top_id)
{
	t_request	popped;

	data->coders[top_id].l_dongle->is_free = 0;
	data->coders[top_id].r_dongle->is_free = 0;
	popped = heap_pop(&data->heap);
	pthread_mutex_lock(&data->coders[popped.coder_id - 1].coder_mutex);
	pthread_cond_signal(&data->coders[popped.coder_id - 1].coder_cond);
	pthread_mutex_unlock(&data->coders[popped.coder_id - 1].coder_mutex);
}

static void	restore_heap(t_heap *heap, t_request *arr, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		heap_push(heap, arr[k]);
		k++;
	}
}

static void	select_dispatchable_coder(t_data *data, t_request *temp_array,
		int *dongle_needed, int *temp_count)
{
	int		top_id;
	long	now;

	while (data->heap.used > 0)
	{
		top_id = data->heap.array[0].coder_id - 1;
		get_elapsed_ms(data->simulation.start_time_tv, &now);
		if (can_dispatch(data, top_id, now, dongle_needed))
		{
			dispatch_coder(data, top_id);
			return ;
		}
		dongle_needed[top_id] = 1;
		dongle_needed[(top_id + 1) % data->args.n_coders] = 1;
		temp_array[(*temp_count)++] = heap_pop(&data->heap);
	}
}

void	process_heap(t_data *data)
{
	t_request	temp_array[250];
	int			dongle_needed[250];
	int			temp_count;

	pthread_mutex_lock(&data->simulation.heap_mutex);
	if (data->heap.used <= 0)
	{
		pthread_mutex_unlock(&data->simulation.heap_mutex);
		return ;
	}
	temp_count = 0;
	while (temp_count < data->args.n_coders)
		dongle_needed[temp_count++] = 0;
	temp_count = 0;
	select_dispatchable_coder(data, temp_array, dongle_needed, &temp_count);
	restore_heap(&data->heap, temp_array, temp_count);
	pthread_mutex_unlock(&data->simulation.heap_mutex);
}
