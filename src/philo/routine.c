/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 16:51:20 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 18:07:43 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*routine(void *vptr);
static void	*process_finish(t_philo_info *info, t_philo *philo, t_stat stat);
static void	*check_finish(void *vptr);

int	start_dine(t_philo_info *info, t_philo *philos)
{
	int			i;
	int			cnt_philo;
	pthread_t	tid;

	cnt_philo = info->arg.cnt_philo;
	info->time_start = get_time();
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_create(&philos[i].tid, NULL, routine, &philos[i]))
			return (philo_err_exit("Failed to create pthread."));
		i++;
	}
	if (pthread_create(&tid, NULL, check_finish, info))
		return (philo_err_exit("Failed to create pthread."));
	if (pthread_detach(tid))
		return (philo_err_exit("Failed to detach pthread."));
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_join(philos[i].tid, NULL))
			return (philo_err_exit("Failed to join pthread."));
		i++;
	}
	return (0);
}

static void	*routine(void *vptr)
{
	t_stat			stat;
	t_philo_info	*info;
	t_philo			*philo;

	philo = (t_philo *)vptr;
	info = philo->info;
	stat = DEF;
	pthread_mutex_lock(&philo->mtx_eat);
	philo->time_last_eat = info->time_start;
	if (info->arg.cnt_philo != 1)
		pthread_mutex_unlock(&philo->mtx_eat);
	while (!is_finish(info))
	{
		if (stat == DEF)
			stat = act_eat(philo, info);
		else if (stat == SLEEP)
			stat = act_sleep(philo, info);
		else if (stat == THINK)
			stat = act_think(philo, info);
	}
	return (NULL);
}

static void	*process_finish(t_philo_info *info, t_philo *philo, t_stat stat)
{
	pthread_mutex_lock(&info->mtx_fin);
	info->finish = true;
	pthread_mutex_unlock(&info->mtx_fin);
	put_msg(info, philo, stat);
	pthread_mutex_unlock(&philo->mtx_eat);
	return (NULL);
}

static void	*check_finish(void *vptr)
{
	int				i;
	int				cnt_full;
	t_philo			*philos;
	t_philo_info	*info;

	info = (t_philo_info *)vptr;
	philos = info->philos;
	while (1)
	{
		ft_sleep(5);
		i = 0;
		cnt_full = 0;
		while (i < info->arg.cnt_philo)
		{
			pthread_mutex_lock(&philos[i].mtx_eat);
			cnt_full += philos[i].full;
			if (is_full(info, cnt_full))
				return (process_finish(info, &philos[i], FULL));
			if (is_dead(info, &philos[i]))
				return (process_finish(info, &philos[i], DEAD));
			pthread_mutex_unlock(&philos[i].mtx_eat);
			i++;
		}
	}
	return (NULL);
}
