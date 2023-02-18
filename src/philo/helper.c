/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:39:48 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/19 01:52:12 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

bool	is_full(t_philo_info *info)
{
	int	i;
	int	cnt_philo;

	i = 0;
	cnt_philo = info->arg.cnt_philo;
	while (i < cnt_philo && info->philo[i].full)
	{
		i++;
		if (i == cnt_philo)
			return (true);
	}
	return (false);
}

bool	check_full(t_philo_info *info, t_philo *philo, t_arg arg)
{
	if (arg.cnt_must_eat > 0 && philo->cnt_eat >= arg.cnt_must_eat)
	{
		philo->full = true;
		if (is_full(info))
			return (true);
		philo->cnt_eat = 0;
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
		usleep(1);
}
