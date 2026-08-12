/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:43:23 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/12 07:08:05 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	initializer(t_data *data)
{	
	data->monitor.compiles = 0;
	data->monitor.game_over = 0;

	if (init_dongles(data) || init_coders(data) || init_heap(data))
		return (1);
	return (0);
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
	// coder_routine(&data.coders[0]);
	return (0);
}
