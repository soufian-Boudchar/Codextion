/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:47:03 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/10 03:50:57 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_args(char **av, t_args *args)
{
	args->n_coders = ft_atoi(av[0]);
	args->time_burnout = ft_atoi(av[1]);
	args->time_compile = ft_atoi(av[2]);
	args->time_debug = ft_atoi(av[3]);
	args->time_refactor = ft_atoi(av[4]);
	args->n_compiles_required = ft_atoi(av[5]);
	args->dongle_cooldown = ft_atoi(av[6]);
	// args->scheduler = av[7];
}

int	scheduler_validator(t_args *args, int *valid, char *scheduler)
{
	if (strcmp(scheduler, "fifo") != 0 && strcmp(scheduler, "edf") != 0)
	{
		error_message(3, scheduler);
		*valid = -1;
	}
	else if (strcmp(scheduler, "fifo") == 0)
		args->sch = 0;
	else if (strcmp(scheduler, "edf") == 0)
	{
		args->sch = 1;
	}
	return (0);
}

int	check_zero(char **av, int *values)
{
	int	valid;

	valid = 0;
	if (values[0] == 0)
		valid = error_message(4, av[0]);
	if (values[5] == 0)
		valid = error_message(5, av[5]);
	return (valid);
}

int	validate_args(char **av, t_args *args)
{
	int	valid;
	int	i;
	int	values[7];

	i = 0;
	init_args(av, args);
	values[0] = args->n_coders;
	values[1] = args->time_burnout;
	values[2] = args->time_compile;
	values[3] = args->time_debug;
	values[4] = args->time_refactor;
	values[5] = args->n_compiles_required;
	values[6] = args->dongle_cooldown;
	valid = check_zero(av, values);
	while ((unsigned long)i < sizeof(values) / sizeof(values[0]))
	{
		if (values[i] == -1)
		{
			error_message(2, av[i]);
			valid = -1;
		}
		i++;
	}
	scheduler_validator(args, &valid, av[7]);
	return (valid);
}
