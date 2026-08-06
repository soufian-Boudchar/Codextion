/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/31 00:42:43 by sboudcha          #+#    #+#             */
/*   Updated: 2026/08/06 00:38:43 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

// this is missed imports and i have to remove its
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_args
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	int				scheduler;
}					t_args;

typedef struct s_dongle
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}					t_dongle;

typedef struct s_coder
{
	int				id;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	int				compiles_count;
	long long		last_compile_time;
	pthread_mutex_t	coder_mutex;

}					t_coder;


typedef struct s_request{
	int coder_id;
	long long priority;
}	t_request;

typedef struct s_heap
{
	t_request *array;
	int size;
	int used;
}	t_heap;

typedef struct s_data
{
	t_args	args;
	t_dongle *dongles;
	t_coder *coders;
	t_heap	heap;
}		t_data;

int					ft_atoi(char *str);
void				init_args(char **av, t_args *args);
int	scheduler_validator(t_args *arg, int *valid, char *scheduler);
int	validate_args(char **av, t_args *args);
int	error_message(int error, char *arg);
int init_dongles(t_data *data);
int init_coders(t_data *data);
int init_heap(t_data *data);

#endif