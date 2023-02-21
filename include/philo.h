/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 20:11:17 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 01:39:00 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <string.h>
# include <time.h>
# include <sys/time.h>
# include "utils.h"

# ifndef PHILO_MAX
 # define PHILO_MAX 200
# endif

typedef enum e_stat
{
	ERR = -1,
	DEF,
	FORK,
	EAT,
	SLEEP,
	THINK,
	FULL,
	DEAD,
}	t_stat;

typedef struct s_arg t_arg;
typedef struct s_philo t_philo;
typedef struct s_philo_info t_philo_info;

struct s_arg
{
	int	cnt_philo;
	int	cnt_must_eat;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
};

struct s_philo
{
	int				id;
	int				cnt_eat;
	int				fork_l;
	int				fork_r;
	bool			full;
	t_stat			stat;
	size_t			time_last_eat;
	pthread_t		tid;
	pthread_mutex_t	mtx_eat;
	t_philo_info	*info;
};

struct s_philo_info
{
	bool			finish;
	size_t			time_start;
	t_arg			arg;
	pthread_mutex_t	mtx_put;
	pthread_mutex_t	mtx_fin;
	pthread_mutex_t	forks[PHILO_MAX];
	t_philo			philos[PHILO_MAX];
};


/* init.c */
int		init_s_philo_info(t_philo_info *info, int argc, char *argv[]);

/* exit.c */
int		philo_err_exit(char *error_msg);
void	destroy_mutex(t_philo_info *info);

/* put.c */
void	put_msg(t_philo_info *info, t_philo *philo, t_stat stat);

/* helper.c */
size_t	get_time(void);
void	check_status(t_philo_info *info, t_philo *philo, t_stat *stat);
void	ft_sleep(size_t	sleep);
//bool	is_finish(t_philo_info *info);
//bool	is_full(t_philo_info *info);
//bool	check_full(t_philo_info *info, t_philo *philo, t_arg arg);
//bool	is_dead(t_philo_info *info);

#endif
