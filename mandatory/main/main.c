/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/11 03:06:21 by hiroaki          ###   ########.fr       */
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
	if (error_msg)
	{
		printf("%s\n", error_msg);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
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

void	get_time(t_time *time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time->s = tv.tv_sec;
	time->ms = tv.tv_usec / 1000;
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
		philo[i].cnt_eat = 0;
		philo[i].fork_l = i;
		philo[i].fork_r = (i + 1) % arg->cnt_philo;
		get_time(&philo[i].time_last_eat);
		if (pthread_mutex_init(&philo[i].eater, NULL) != SUCCESS)
			philo_exit(d, "Mutex failure.");
	}
}

t_data	*init_s_data(t_data *d)
{
	d = (t_data *)malloc(sizeof(t_data));
	if (!d)
		philo_exit(d, "Malloc failure.");
	return (d);
}

void	philo_dine()
{

}

void	routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	get_time(&philo->time_last_eat);
	while (philo->status != FULL && philo->status != DEAD)
	{
		philo_dine();
	}
}

void	confirmation_survival(void *p)
{

}

void	start_dine(t_data *d, t_arg *arg, t_philo *philo)
{
	int			i;
	pthread_t	tid;

	get_time(&d->time_start);
	i = -1;
	while (++i < arg->cnt_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, routine, &philo[i]) != SUCCESS)
			philo_exit(d, "Failed to create pthread.");
	}
	if (pthread_create(&tid, NULL, confirmation_survival, philo) != SUCCESS)
		philo_exit(d, "Failed to create pthread.");
	if (pthread_detach(tid) != SUCCESS)
		philo_exit(d, "Failed to detach pthread.");
	i = -1;
	while (++i < arg->cnt_philo)
	{
		if (pthread_join(&philo[i].tid, NULL) != SUCCESS)
			philo_exit(d, "Failed to join pthread.");
	}
}

void	init_mutex(t_data *d, t_arg *arg)
{
	int	i;

	d->forks = malloc(sizeof(pthread_mutex_t) * arg->cnt_philo);
	if (!d->forks)
		philo_exit(d, "Malloc failure.");
	i = -1;
	while (++i < arg->cnt_philo)
	{
		if (pthread_mutex_init(&d->forks[i], NULL) != SUCCESS)
			philo_exit(d, "Failed to create pthread.");
	}
}

int	main(int argc, char *argv[])
{
	t_data	*d;

	d = init_s_data(d);
	init_s_arg(d, &d->arg, argc, argv);
	init_s_philo(d, &d->arg, d->philo);
	init_mutex(d, &d->arg);
	start_dine(d, &d->arg, d->philo);
	philo_exit(d, NULL);
}
