/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sboudcha <sboudcha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 19:38:09 by sboudcha          #+#    #+#             */
/*   Updated: 2026/09/02 19:38:40 by sboudcha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;

typedef struct s_args
{
	int					n_coders;
	int					n_compiles_required;
	int					time_burnout;
	int					time_compile;
	int					time_debug;
	int					time_refactor;
	int					dongle_cooldown;
	int					sch;
}						t_args;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int					is_free;
	long				cooldown;
	long				available_at;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	pthread_t			tid;
	t_data				*data;
	pthread_cond_t		coder_cond;
	pthread_mutex_t		coder_mutex;
	pthread_mutex_t		time_mutex;
	pthread_mutex_t		compiles_mutex;
	t_dongle			*l_dongle;
	t_dongle			*r_dongle;
	int					compiles_count;
	struct timeval		lst_compile_tv;

}						t_coder;

typedef struct s_request
{
	int					coder_id;
	long				priority;
}						t_request;

typedef struct s_heap
{
	int					size;
	int					used;
	t_request			*array;
}						t_heap;

typedef struct s_simulation
{
	pthread_mutex_t		timer_mutex;
	pthread_mutex_t		heap_mutex;
	struct timeval		start_time_tv;
	// long				current_time_ms;
	struct timeval		current_time_tv;
	pthread_mutex_t		current_time_mutex;

}						t_simulation;

typedef struct s_monitor
{
	int					game_over;
	long				burnout_timer;
	pthread_mutex_t		game_over_mutex;
	struct timeval		timer_tv;
	long				timer_ms;
	int					compiles;

}						t_monitor;
typedef struct s_data
{
	t_args				args;
	t_dongle			*dongles;
	t_coder				*coders;
	t_heap				heap;
	t_simulation		simulation;
	t_monitor			monitor;
}						t_data;

void					process_heap(t_data *data);
void					join_coders(t_data *data);
int						init_mutex(t_data *data);
int						check_game_over(t_data *data);
void					get_elapsed_ms(struct timeval start, long *time_ms);
int						ft_atoi(char *str);
int						monitor(t_data *data);
void					init_args(char **av, t_args *args);
int						scheduler_validator(t_args *arg, int *valid,
							char *scheduler);
int						validate_args(char **av, t_args *args);
int						error_message(int error, char *arg);
int						init_dongles(t_data *data);
int						init_coders(t_data *data);
int						init_heap(t_data *data);
void					*coder_routine(void *coder_ptr);
int						monitor(t_data *data);
t_request				heap_pop(t_heap *heap);
void					heap_push(t_heap *heap, t_request new_req);
void					free_dongles(t_data *data);
#endif