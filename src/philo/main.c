/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/19 02:18:46 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_dine(t_philo_info *info, t_philo *philo)
{
	pthread_mutex_lock(&info->forks[philo->fork_r]);
	pthread_mutex_lock(&info->forks[philo->fork_l]);
	put_msg(info, philo, FORK);
	put_msg(info, philo, FORK);
	pthread_mutex_lock(&philo->monitor_eat);
	put_msg(info, philo, EAT);
	philo->time_last_eat = get_time();
	philo->cnt_eat++;
	check_full(info, philo, info->arg);
	pthread_mutex_unlock(&philo->monitor_eat);
	ft_sleep(info->arg.time_to_eat);
	pthread_mutex_unlock(&info->forks[philo->fork_r]);
	pthread_mutex_unlock(&info->forks[philo->fork_l]);
}

static void	*routine(void *vptr)
{
	t_philo			*philo;
	t_philo_info	*info;
	t_arg			arg;

	philo = (t_philo *)vptr;
	info = philo->info;
	arg = info->arg;
	if (arg.cnt_philo == 1)
		ft_sleep(arg.time_to_die + 2);
	if (philo->id % 2)
		ft_sleep(2);
	philo->time_last_eat = get_time();
	while (!info->finish)
	{
		philo_dine(philo->info, philo);
		if (check_full(info, philo, arg))
			break ;
		put_msg(info, philo, SLEEP);
		ft_sleep(arg.time_to_sleep);
		put_msg(info, philo, THINK);
	}
	return (NULL);
}

static void	*check_finish(void *vptr)
{
	t_philo_info	*info;

	info = (t_philo_info *)vptr;
	while (!info->finish)
	{
		if (is_full(info))
			return (put_msg(info, info->philo, FULL));
		if (is_dead(info))
			return (put_msg(info, info->philo, DEAD));
		ft_sleep(1);
	}
	return (NULL);
}

static void	start_dine(t_philo_info *info, t_philo *philo)
{
	int			i;
	int			cnt_philo;
	pthread_t	tid;

	cnt_philo = info->arg.cnt_philo;
	info->time_start = get_time();
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, routine, &philo[i]))
			philo_err_exit("Failed to create pthread.");
		i++;
	}
	if (pthread_create(&tid, NULL, check_finish, info))
		philo_err_exit("Failed to create pthread.");
	if (pthread_detach(tid))
		philo_err_exit("Failed to detach pthread.");
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_join(philo[i].tid, NULL))
			philo_err_exit("Failed to join pthread.");
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_philo_info	*info;

	init_s_philo_info(&info);
	init_s_arg(&info->arg, argc, argv);
	init_s_philo(info);
	init_mutex(info);
	start_dine(info, info->philo);
	destroy_mutex(info);
	free_all_struct(info);
	return (0);
}

//__attribute__((destructor)) static void destructor()
//{
//	system("leaks -q philo");
//}
