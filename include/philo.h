/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 20:11:17 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/18 14:21:53 by hiroaki          ###   ########.fr       */
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
	FORK,
	EAT,
	SLEEP,
	THINK,
	FULL,
	DEAD,
}	t_sig;

typedef struct s_philo_info	t_philo_info;

typedef struct s_arg
{
	int		cnt_philo;
	int		cnt_must_eat;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
}	t_arg;

//typedef struct s_time
//{
//	int			ms;
//	long int	s;
//}	t_time;

typedef struct s_philo
{
	int				id;
	int				cnt_eat;
	int				fork_l;
	int				fork_r;
	size_t			time_last_eat;
	pthread_t		tid;
	pthread_mutex_t	monitor_eat;
	t_philo_info	*info;
}	t_philo;

typedef struct s_philo_info
{
	int				cnt_full;
	int				philo_status;
	size_t			time_start;
	bool			finish;
	t_arg			arg;
	t_philo			*philo;
	pthread_mutex_t	monitor_output;
	pthread_mutex_t	monitor_finish;
	pthread_mutex_t	*forks;
}	t_philo_info;

size_t	get_time(void);

#endif
