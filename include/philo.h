/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 20:11:17 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/11 03:06:41 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include "utils.h"

# define SUCCESS 0
# define DEAD 1
# define FULL 2

typedef struct s_arg {
	int		cnt_philo;
	int		cnt_must_eat;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
}	t_arg;

typedef struct s_time {
	int			ms;
	long int	s;
}	t_time;

typedef struct s_philo {
	int				id;
	int				cnt_eat;
	int				status;
	int				fork_l;
	int				fork_r;
	t_time			time_last_eat;
	pthread_t		tid;
	pthread_mutex_t	eater;
}	t_philo;

typedef struct s_data {
	int				cnt_full;
	t_arg			arg;
	t_time			time_start;
	t_philo			*philo;
	pthread_mutex_t	*forks;
}	t_data;

#endif
