/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:43:23 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:23:55 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

static void	one_coder(t_data *data)
{
	t_coder	coder;

	coder = data->coders[0];
	coder.l_dongle->is_free = 0;
	printf("0 1 has taken a dongle\n");
	usleep(data->args.time_burnout * 1000);
	printf("%d %d burned out\n", (data->args.time_burnout), coder.id);
}

static int	initializer(t_data *data)
{
	data->monitor.compiles = 0;
	data->monitor.game_over = 0;
	data->dongles = NULL;
	data->coders = NULL;
	data->heap.array = NULL;
	if (init_dongles(data) || init_coders(data) || init_heap(data))
		return (1);
	init_mutex(data);
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
	if (initializer(&data))
	{
		free_all(&data);
		return (1);
	}
	if (data.args.n_coders == 1)
	{
		one_coder(&data);
		free_all(&data);
		return (0);
	}
	monitor(&data);
	free_all(&data);
	return (0);
}
