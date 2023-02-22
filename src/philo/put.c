/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:50:01 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 16:50:21 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_stat	put_msg(t_philo_info *info, t_philo *philo, t_stat stat)
{
	int		id;
	size_t	time;

	id = philo->id + 1;
	pthread_mutex_lock(&info->mtx_put);
	time = get_time() - info->time_start;
	if (stat == FULL)
		printf("%47s\n", "All philosophers are full :))");
	if (stat == DEAD)
		printf("%10zdms =>   philo_%-4d is died\n", time, id);
	if (!is_finish(info))
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
	return (stat);
}
