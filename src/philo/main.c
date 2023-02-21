/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/20 17:09:30hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	act_eat(t_philo *philo, t_stat *stat)
{
	int				must_eat;
	t_philo_info	*info;

	*stat = SLEEP;
	info = philo->info;
	if (philo->id % 2)
		usleep(100);
	pthread_mutex_lock(&info->forks[philo->fork_r]);
	put_msg(info, philo, FORK);
	if (philo->fork_r == philo->fork_l)
	{
		*stat = DEAD;
		return ;
	}
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
}

void	act_sleep(t_philo *philo, t_stat *stat)
{
	t_philo_info	*info;

	*stat = THINK;
	info = philo->info;
	put_msg(info, philo, SLEEP);
	ft_sleep(info->arg.time_to_sleep);
}

void	act_think(t_philo *philo, t_stat *stat)
{
	t_philo_info	*info;

	info = philo->info;
	put_msg(info, philo, THINK);
	*stat = DEF;
}

static void	*routine(void *vptr)
{
	t_stat			stat;
	t_philo_info	*info;
	t_philo			*philo;

	philo = (t_philo *)vptr;
	info = philo->info;
	stat = DEF;
	if (philo->id % 2)
		usleep(500);
	while (!info->finish)
	{
		if (stat == DEF)
			act_eat(philo, &stat);
		else if (stat == SLEEP)
			act_sleep(philo, &stat);
		else if (stat == THINK)
			act_think(philo, &stat);
	}
	return (NULL);
}

static void	*check_finish(void *vptr)
{
	t_stat			stat;
	t_philo_info	*info;

	info = (t_philo_info *)vptr;
	stat = DEF;
	while (1)
	{
		check_status(info, info->philos, &stat);
		if (stat != DEF)
		{
			pthread_mutex_lock(&info->mtx_fin);
			info->finish = true;
			pthread_mutex_unlock(&info->mtx_fin);
			break ;
		}
		ft_sleep(5);
	}
	return (NULL);
}

static int	start_dine(t_philo_info *info, t_philo *philos)
{
	int			i;
	int			cnt_philo;
	pthread_t	tid;

	cnt_philo = info->arg.cnt_philo;
	info->time_start = get_time();
	i = 0;
	while (i < cnt_philo)
	{
		philos[i].time_last_eat = info->time_start;
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
		//if (pthread_detach(philos[i].tid))
		//	return (philo_err_exit("Failed to detach pthread."));
		if (pthread_join(philos[i].tid, NULL))
			return (philo_err_exit("Failed to join pthread."));
		i++;
	}
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo_info	info;

	if (init_s_philo_info(&info, argc, argv) || \
		start_dine(&info, info.philos))
		return (EXIT_FAILURE);
	destroy_mutex(&info);
	return (EXIT_SUCCESS);
}

//__attribute__((destructor)) static void destructor()
//{
//	system("leaks -q philo");
//}
