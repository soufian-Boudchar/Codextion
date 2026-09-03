/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:43:23 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 03:41:25 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	initializer(t_data *data)
{
	data->monitor.compiles = 0;
	data->monitor.game_over = 0;
	if (init_dongles(data) || init_coders(data) || init_heap(data)
		|| init_mutex(data))
		return (1);
	return (0);
}

void	free_all(t_data *data)
{
	free(data->dongles);
	free(data->coders);
	free(data->heap.array);
}

int	main(int ac, char *av[])
{
	t_data	data;

	if (ac != 9)
		return (error_message(1, NULL));
	if (validate_args(av + 1, &data.args) == -1)
		return (1);
	initializer(&data);
	monitor(&data);
	free_all(&data);
	return (0);
}
