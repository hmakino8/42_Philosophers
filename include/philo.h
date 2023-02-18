/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 20:11:17 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/19 01:43:00 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include "utils.h"

typedef enum e_sig
{
	FORK = 1,
	EAT,
	SLEEP,
	THINK,
	FULL,
	DEAD,
}	t_sig;

typedef struct s_philo_info	t_philo_info;

typedef struct s_arg
{
	int	cnt_philo;
	int	cnt_must_eat;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
}	t_arg;

typedef struct s_philo
{
	int				id;
	int				cnt_eat;
	int				fork_l;
	int				fork_r;
	bool			full;
	size_t			time_last_eat;
	pthread_t		tid;
	pthread_mutex_t	monitor_eat;
	t_philo_info	*info;
}	t_philo;

typedef struct s_philo_info
{
	int				philo_status;
	size_t			time_start;
	bool			finish;
	t_arg			arg;
	t_philo			*philo;
	pthread_mutex_t	monitor_output;
	pthread_mutex_t	monitor_finish;
	pthread_mutex_t	*forks;
}	t_philo_info;

/* init.c */
void	init_s_arg(t_arg *arg, int argc, char *argv[]);
void	init_s_philo(t_philo_info *info);
void	init_s_philo_info(t_philo_info **info);
void	init_mutex(t_philo_info *info);

/* exit.c */
void	free_all_struct(t_philo_info *info);
void	destroy_mutex(t_philo_info *info);
void	philo_err_exit(char *error_msg);

/* put.c */
void	*put_msg(t_philo_info *info, t_philo *philo, int sig);

/* helper.c */
size_t	get_time(void);
bool	is_full(t_philo_info *info);
bool	check_full(t_philo_info *info, t_philo *philo, t_arg arg);
bool	is_dead(t_philo_info *info);
void	ft_sleep(size_t	sleep);

#endif
