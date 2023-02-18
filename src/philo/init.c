/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:32:15 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/19 01:30:25 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_s_arg(t_arg *arg, int argc, char *argv[])
{
	if (argc == 5 || argc == 6)
	{
		arg->cnt_philo = ft_atoi(argv[1]);
		arg->time_to_die = ft_atoi(argv[2]);
		arg->time_to_eat = ft_atoi(argv[3]);
		arg->time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			arg->cnt_must_eat = ft_atoi(argv[5]);
		else
			arg->cnt_must_eat = 0;
	}
	if (errno == EINVAL || errno == ERANGE || \
		arg->cnt_philo < 1 || \
		arg->time_to_die < 1 || \
		arg->time_to_eat < 1 || \
		arg->time_to_sleep < 1 || \
		arg->cnt_must_eat < 0)
		philo_err_exit("Invalid argument.");
}

void	init_s_philo(t_philo_info *info)
{
	int	i;
	int	cnt_philo;

	cnt_philo = info->arg.cnt_philo;
	info->philo = malloc(sizeof(t_philo) * cnt_philo);
	if (info->philo == NULL)
		philo_err_exit("Malloc failure.");
	i = 0;
	while (i < cnt_philo)
	{
		info->philo[i].id = i;
		info->philo[i].cnt_eat = 0;
		info->philo[i].fork_l = i;
		info->philo[i].fork_r = (i + 1) % cnt_philo;
		info->philo[i].info = info;
		info->philo[i].full = false;
		info->philo[i].time_last_eat = get_time();
		pthread_mutex_init(&info->philo[i].monitor_eat, NULL);
		i++;
	}
}

void	init_s_philo_info(t_philo_info **info)
{
	*info = malloc(sizeof(t_philo_info));
	if (*info == NULL)
		philo_err_exit("Malloc failure.");
	(*info)->philo_status = 0;
	(*info)->finish = false;
}

void	init_mutex(t_philo_info *info)
{
	int	i;
	int	cnt_philo;

	cnt_philo = info->arg.cnt_philo;
	info->forks = malloc(sizeof(pthread_mutex_t) * cnt_philo);
	if (info->forks == NULL)
		philo_err_exit("Malloc failure.");
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
			philo_err_exit("Failed to init mutex");
		i++;
	}
	if (pthread_mutex_init(&info->monitor_output, NULL))
		philo_err_exit("Failed to init mutex");
}
