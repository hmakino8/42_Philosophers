/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 17:00:02 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 17:00:12 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_s_arg(t_arg *arg, int argc, char *argv[]);
static int	init_mutex(t_philo_info *info);

int	init_s_philo_info(t_philo_info *info, int argc, char *argv[])
{
	int		i;
	t_arg	arg;

	if (init_s_arg(&arg, argc, argv))
		return (philo_err_exit("Invalid argument\n"));
	info->finish = false;
	info->arg = arg;
	i = 0;
	while (i < arg.cnt_philo)
	{
		info->philos[i].id = i;
		info->philos[i].cnt_eat = 0;
		info->philos[i].fork_r = i;
		info->philos[i].fork_l = (i + 1) % arg.cnt_philo;
		info->philos[i].info = info;
		info->philos[i].full = false;
		i++;
	}
	if (init_mutex(info))
		return (philo_err_exit("Failed to init mutex\n"));
	return (0);
}

static int	init_s_arg(t_arg *arg, int argc, char *argv[])
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
		arg->cnt_philo > 200 || \
		arg->time_to_die < 1 || \
		arg->time_to_eat < 1 || \
		arg->time_to_sleep < 1 || \
		arg->cnt_must_eat < 0)
		return (1);
	return (0);
}

static int	init_mutex(t_philo_info *info)
{
	int	i;

	i = 0;
	while (i < info->arg.cnt_philo)
	{
		if (pthread_mutex_init(&info->forks[i], NULL) || \
			pthread_mutex_init(&info->philos[i].mtx_eat, NULL))
			return (1);
		i++;
	}
	if (pthread_mutex_init(&info->mtx_put, NULL) || \
		pthread_mutex_init(&info->mtx_fin, NULL))
		return (1);
	return (0);
}
