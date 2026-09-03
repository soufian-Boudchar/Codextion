/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:56:49 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/03 07:24:10 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/codexion.h"

void	join_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->args.n_coders)
	{
		pthread_join(data->coders[i].tid, NULL);
		i++;
	}
}

int	ft_atoi(char *str)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		res = (res * 10) + (*str - '0');
		if ((res * sign) < 0 || res * sign > INT_MAX)
			return (-1);
		str++;
	}
	return ((int)res * sign);
}

int	check_game_over(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->monitor.game_over_mutex);
	status = data->monitor.game_over;
	pthread_mutex_unlock(&data->monitor.game_over_mutex);
	return (status);
}

int	error_message(int error, char *arg)
{
	if (error == 1)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m Invalid number of parameters."
			" Expected exactly 8 parameters.\n");
	else if (error == 2)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m Invalid value \"%s\"."
			" Expected a positive integer value.\n", arg);
	else if (error == 3)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m"
			" Invalid scheduler \"%s\"."
			" Expected \"edf\" or \"fifo\".\n", arg);
	else if (error == 4)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m"
			" Invalid number of coders \'%s\'."
			" Expected a value greater than 0.\n", arg);
	else if (error == 5)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m"
			" Invalid number of compiles required \'%s\'"
			". Expected a value greater than 0.\n", arg);
	return (-1);
}

void	get_elapsed_ms(struct timeval start, long *time_ms)
{
	struct timeval	end;

	gettimeofday(&end, NULL);
	*time_ms = (((end.tv_sec - start.tv_sec) * 1000) + ((end.tv_usec
					- start.tv_usec) / 1000));
}
