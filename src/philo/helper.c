/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:39:48 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 17:00:48 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	ft_sleep(size_t	sleep)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < sleep)
		usleep(500);
}

bool	is_full(t_philo_info *info, int full_cnt)
{
	int	cnt_philo;

	cnt_philo = info->arg.cnt_philo;
	return (full_cnt == cnt_philo);
}

bool	is_dead(t_philo_info *info, t_philo *philo)
{
	size_t	time_to_die;
	size_t	time_last_eat;

	time_to_die = info->arg.time_to_die;
	time_last_eat = philo->time_last_eat;
	return (get_time() - time_last_eat > time_to_die);
}

bool	is_finish(t_philo_info *info)
{
	bool	ok;

	pthread_mutex_lock(&info->mtx_fin);
	ok = info->finish;
	pthread_mutex_unlock(&info->mtx_fin);
	return (ok);
}
