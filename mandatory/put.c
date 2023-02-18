/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:36:44 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/18 22:10:43 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	put_final_msg(t_philo_info *info, size_t time, int sig, int id)
{
	if (sig == FULL)
	{
		if (info->finish)
			return ;
		pthread_mutex_lock(&info->monitor_finish);
		info->finish = true;
		pthread_mutex_unlock(&info->monitor_finish);
		printf("%47s\n", "All philosophers are full :))");
	}
	else
		printf("%10zdms =>   philo_%-4d is died\n", time, id);
}

void	put_msg(t_philo_info *info, t_philo *philo, int sig)
{
	int		id;
	size_t	time;

	id = philo->id + 1;
	time = get_time() - info->time_start;
	pthread_mutex_lock(&info->monitor_output);
	if (sig == DEAD || sig == FULL)
		put_final_msg(info, time, sig, id);
	else if (!info->finish)
	{
		if (sig == FORK)
			printf("%10zdms =>   philo_%-4d has taken a fork\n", time, id);
		if (sig == EAT)
			printf("%10zdms =>   philo_%-4d is eating\n", time, id);
		if (sig == SLEEP)
			printf("%10zdms =>   philo_%-4d is sleeping\n", time, id);
		if (sig == THINK)
			printf("%10zdms =>   philo_%-4d is thinking\n", time, id);
	}
	pthread_mutex_unlock(&info->monitor_output);
}
