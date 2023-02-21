/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:39:48 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 01:42:06 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	check_status(t_philo_info *info, t_philo *philos, t_stat *stat)
{
	int		i;
	int		full_cnt;
	int		cnt_philo;
	size_t	time_to_die;

	cnt_philo = info->arg.cnt_philo;
	time_to_die = info->arg.time_to_die;
	i = 0;
	full_cnt = 0;
	while (i < cnt_philo)
	{
		if (get_time() - philos[i].time_last_eat > time_to_die)
		{
			*stat = DEAD;
			printf("%ld\n", get_time() - philos[i].time_last_eat);
			return (put_msg(info, &info->philos[i], *stat));
		}
		if (philos[i].full)
			full_cnt++;
		i++;
	}
	if (full_cnt == cnt_philo)
	{
		*stat = FULL;
		return (put_msg(info, &info->philos[i], *stat));
	}
}

void	ft_sleep(size_t	sleep)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < sleep)
		usleep(500);
}
