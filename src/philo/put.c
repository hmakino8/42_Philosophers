/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:36:44 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 01:37:27 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_msg(t_philo_info *info, t_philo *philo, t_stat stat)
{
	int		id;
	size_t	time;

	id = philo->id;
	time = get_time() - info->time_start;
	pthread_mutex_lock(&info->mtx_put);
	if (stat == FULL)
		printf("%47s\n", "All philosophers are full :))");
	if (stat == DEAD)
		printf("%10zdms =>   philo_%-4d is died\n", time, id);
	if (!info->finish)
	{
		if (stat == FORK)
			printf("%10zdms =>   philo_%-4d has taken a fork\n", time, id);
		if (stat == EAT)
			printf("%10zdms =>   philo_%-4d is eating\n", time, id);
		if (stat == SLEEP)
			printf("%10zdms =>   philo_%-4d is sleeping\n", time, id);
		if (stat == THINK)
			printf("%10zdms =>   philo_%-4d is thinking\n", time, id);
	}
	pthread_mutex_unlock(&info->mtx_put);
}
