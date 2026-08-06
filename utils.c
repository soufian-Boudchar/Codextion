/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:56:49 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/03 15:22:32 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	error_message(int error, char *arg)
{
	if (error == 1)
		fprintf(stderr, "\033[31m[ERROR]\033[0m"
			" Invalid number of parameters."
			" Expected exactly 8 parameters.\n");
	else if (error == 2)
		fprintf(stderr,
			"\033[31m[ERROR]\033[0m"
			" Invalid value \"%s\"."
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
