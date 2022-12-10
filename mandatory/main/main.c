/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/10 20:38:44 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

void	philo_exit(t_data *d, char *error_msg)
{
	if (d)
	{
		if (d->philo)
			free(d->philo);
		free(d);
	}
	printf("%s\n", error_msg);
	exit(EXIT_FAILURE);
}

void	init_s_arg(t_data *d, t_arg *arg, int argc, char *argv[])
{
	bool	ok;

	ok = false;
	if (argc == 5 || argc == 6)
	{
		arg->cnt_philo = ft_atoi(argv[1], &ok);
		arg->time_to_die = ft_atoi(argv[2], &ok);
		arg->time_to_eat = ft_atoi(argv[3], &ok);
		arg->time_to_sleep = ft_atoi(argv[4], &ok);
		if (argc == 6)
			arg->cnt_must_eat = ft_atoi(argv[5], &ok);
		else
			arg->cnt_must_eat = 0;
	}
	if (!ok)
		philo_exit(d, "Invalid argument.");
}

void	init_s_time(t_data *d, t_time *time)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		philo_exit(d, "Failed to get time.");
	time->s = tv.tv_sec;
	time->ms = tv.tv_usec / 1000;
}

void	init_fork(t_arg *arg, t_philo *philo, int i)
{
	if (i % 2 == 0)
	{
		philo->fork_r = true;
		philo->fork_l = true;
	}
	else
	{
		philo->fork_r = false;
		philo->fork_l = false;
	}
	if (i == arg->cnt_philo - 1)
		philo->fork_l = false;
}

void	init_s_philo(t_data *d, t_arg *arg, t_philo *philo)
{
	int	i;

	philo = (t_philo *)malloc(sizeof(t_philo) * arg->cnt_philo);
	if (!philo)
		philo_exit(d, "Malloc failure.");
	i = -1;
	while (++i < arg->cnt_philo)
	{
		philo[i].id = i;
		init_fork(arg, &philo[i], i);
		if (pthread_mutex_init(&philo[i].monitor_eat, NULL) == ERROR)
			philo_exit(d, "Mutex failure.");
		philo[i].cnt_eat = 0;
		init_s_time(d, &philo[i].time_last_eat);
		//printf("philo[%d] = %ld%03d\n", i, philo[i].time_last_eat.s, philo[i].time_last_eat.ms);
	}
}

t_data	*init_s_data(t_data *d)
{
	d = (t_data *)malloc(sizeof(t_data));
	if (!d)
		philo_exit(d, "Malloc Failure.");
	return (d);
}

int	main(int argc, char *argv[])
{
	t_data	*d;

	d = init_s_data(d);
	init_s_arg(d, &d->arg, argc, argv);
	init_s_time(d, &d->time_start);
	init_s_philo(d, &d->arg, d->philo);
}
