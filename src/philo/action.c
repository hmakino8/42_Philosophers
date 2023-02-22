/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:16:16 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 15:16:34 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_stat	act_eat(t_philo *philo, t_philo_info *info)
{
	int	must_eat;

	if (philo->id % 2)
		usleep(500);
	pthread_mutex_lock(&info->forks[philo->fork_r]);
	put_msg(info, philo, FORK);
	if (philo->fork_r == philo->fork_l)
		return (DEAD);
	pthread_mutex_lock(&info->forks[philo->fork_l]);
	put_msg(info, philo, FORK);
	pthread_mutex_lock(&philo->mtx_eat);
	put_msg(info, philo, EAT);
	philo->time_last_eat = get_time();
	philo->cnt_eat++;
	must_eat = info->arg.cnt_must_eat;
	if (must_eat > 0 && philo->cnt_eat >= must_eat)
		philo->full = true;
	pthread_mutex_unlock(&philo->mtx_eat);
	ft_sleep(info->arg.time_to_eat);
	pthread_mutex_unlock(&info->forks[philo->fork_r]);
	pthread_mutex_unlock(&info->forks[philo->fork_l]);
	return (SLEEP);
}

t_stat	act_sleep(t_philo *philo, t_philo_info *info)
{
	put_msg(info, philo, SLEEP);
	ft_sleep(info->arg.time_to_sleep);
	return (THINK);
}

t_stat	act_think(t_philo *philo, t_philo_info *info)
{
	put_msg(info, philo, THINK);
	return (DEF);
}
