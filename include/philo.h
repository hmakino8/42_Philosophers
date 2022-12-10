/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 20:11:17 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/10 20:31:31 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO
# define PHILO

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include "utils.h"

# define ERROR -1

typedef struct s_arg
{
	int		cnt_philo;
	int		cnt_must_eat;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
}	t_arg;

typedef	struct s_time
{
	long int	s;
	int			ms;
}	t_time;

typedef struct s_philo
{
	int				id;
	int				cnt_eat;
	bool			fork_l;
	bool			fork_r;
	t_time			time_last_eat;
	pthread_t		thread;
	pthread_mutex_t	monitor_eat;
}	t_philo;

typedef struct s_data
{
	t_arg	arg;
	t_time	time_start;
	t_philo	*philo;
}	t_data;

#endif
