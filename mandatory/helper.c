/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:39:48 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/18 21:42:20 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

bool	is_full(t_philo_info *info, t_philo *philo, t_arg arg)
{
	if (arg.cnt_must_eat > 0 && philo->cnt_eat >= arg.cnt_must_eat)
	{
		pthread_mutex_lock(&philo->monitor_eat);
		info->cnt_full++;
		pthread_mutex_unlock(&philo->monitor_eat);
		if (info->cnt_full == info->arg.cnt_philo)
			put_msg(info, info->philo, FULL);
		if (info->cnt_full >= info->arg.cnt_philo)
			return (true);
	}
	return (false);
}

bool	is_dead(t_philo_info *info)
{
	t_arg	arg;
	t_philo	*philo;

	arg = info->arg;
	philo = info->philo;
	return ((get_time() - philo->time_last_eat) > (size_t)arg.time_to_die);
}

void	ft_sleep(size_t	sleep)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < sleep)
		usleep(10);
}

void	active_monitor_finish(t_philo_info *info, int status)
{
	put_msg(info, info->philo, status);
	pthread_mutex_lock(&info->monitor_finish);
	info->finish = true;
	pthread_mutex_unlock(&info->monitor_finish);
}
