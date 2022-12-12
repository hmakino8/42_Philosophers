/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/12 23:48:14 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

void	free_all_struct(t_data *d)
{
	free(d->philo);
	free(d->forks);
	free(d);
}

void	destroy_mutex(t_data *d)
{
	int	i;

	i = -1;
	while (++i < d->arg.cnt_philo)
		pthread_mutex_destroy(&d->forks[i]);
	pthread_mutex_destroy(&d->monitor_finish);
	pthread_mutex_destroy(&d->monitor_output);
}

void	philo_exit(t_data *d, char *error_msg)
{
	if (d)
	{
		destroy_mutex(d);
		free_all_struct(d);
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
	if (!ok || \
		arg->cnt_philo < 2 || \
		arg->time_to_die < 1 || \
		arg->time_to_eat < 1 || \
		arg->time_to_sleep < 1)
		philo_exit(d, "Invalid argument.");
}

void	get_time(t_time *time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	time->s = tv.tv_sec;
	time->ms = tv.tv_usec / 1000;
}

void	init_s_philo(t_data *d, t_arg *arg)
{
	int	i;

	d->philo = (t_philo *)malloc(sizeof(t_philo) * arg->cnt_philo);
	if (!d->philo)
		philo_exit(d, "Malloc failure.");
	i = -1;
	while (++i < arg->cnt_philo)
	{
		d->philo[i].id = i;
		d->philo[i].cnt_eat = 0;
		d->philo[i].fork_l = i;
		d->philo[i].fork_r = (i + 1) % arg->cnt_philo;
		d->philo[i].d = d;
		get_time(&d->philo[i].time_last_eat);
		pthread_mutex_init(&d->philo[i].monitor_eat, NULL);
	}
}

t_data	*init_s_data(t_data *d)
{
	d = (t_data *)malloc(sizeof(t_data));
	if (!d)
		philo_exit(d, "Malloc failure.");
	d->cnt_full = 0;
	d->philo_status = CONTINUE;
	return (d);
}

void	put_status_message(t_data *d, t_philo *philo, char *msg)
{
	int			id;
	int			ms;
	long int	sec;
	t_time		now;

	pthread_mutex_lock(&d->monitor_output);
	id = philo->id + 1;
	get_time(&now);
	if (msg)
		printf("%ld%d philo_%d %s\n", now.s, now.ms, id, msg);
	else
		printf("%ld%d %s\n", now.s, now.ms, "All philosophers are full");
	pthread_mutex_unlock(&d->monitor_output);
}

void	philo_dine(t_data *d, t_philo *philo)
{
	pthread_mutex_lock(&d->forks[philo->fork_r]);
	pthread_mutex_lock(&d->forks[philo->fork_l]);
	put_status_message(d, philo, "has taken a fork");
	put_status_message(d, philo, "has taken a fork");
	pthread_mutex_lock(&philo->monitor_eat);
	get_time(&philo->time_last_eat);
	philo->cnt_eat++;
	put_status_message(d, philo, "is eating");
	pthread_mutex_unlock(&philo->monitor_eat);
	usleep(d->arg.time_to_eat);
	pthread_mutex_unlock(&d->forks[philo->fork_r]);
	pthread_mutex_unlock(&d->forks[philo->fork_l]);
}

void	*active_monitor_finish(t_data *d, int status)
{
	if (status == FULL)
		put_status_message(d, d->philo, NULL);
	if (status == DEAD)
		put_status_message(d, d->philo, "is died");
	pthread_mutex_lock(&d->monitor_finish);
	d->philo_status = status;
	pthread_mutex_unlock(&d->monitor_finish);
	return (NULL);
}

void	*routine(void *p)
{
	t_philo	*philo;
	t_data	*d;
	t_arg	arg;

	philo = (t_philo *)p;
	d = philo->d;
	arg = d->arg;
	if (philo->id % 2)
		usleep(200);
	get_time(&philo->time_last_eat);
	while (d->philo_status == CONTINUE)
	{
		philo_dine(philo->d, philo);
		if (philo->cnt_eat == arg.cnt_must_eat)
			if (++d->cnt_full == arg.cnt_philo)
				return (active_monitor_finish(d, FULL));
		put_status_message(d, philo, "is sleeping");
		usleep(arg.time_to_sleep);
		put_status_message(d, philo, "is thinking");
	}
	return (NULL);
}

bool	is_dead(t_data *d)
{
	t_arg	arg;
	t_philo	*philo;
	t_time	now;
	t_time	last_eat;


	get_time(&now);
	arg = d->arg;
	philo = d->philo;
	return (now.s == philo->time_last_eat.s && \
			now.ms - philo->time_last_eat.ms <= arg.time_to_die);
}

void	*confirmation_survival(void *p)
{
	t_philo	*philo;
	t_data	*d;
	t_arg	arg;

	philo = (t_philo *)p;
	d = philo->d;
	arg = d->arg;
	while (d->philo_status == CONTINUE && !is_dead(d))
		;
	return (active_monitor_finish(d, DEAD));
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
		if (pthread_join(philo[i].tid, NULL) != SUCCESS)
			philo_exit(d, "Failed to join pthread.");
}

void	init_mutex(t_data *d, t_arg *arg)
{
	int	i;

	d->forks = malloc(sizeof(pthread_mutex_t) * arg->cnt_philo);
	if (!d->forks)
		philo_exit(d, "Malloc failure.");
	i = -1;
	while (++i < arg->cnt_philo)
		pthread_mutex_init(&d->forks[i], NULL);
	pthread_mutex_init(&d->monitor_finish, NULL);
	pthread_mutex_init(&d->monitor_output, NULL);
}

int	main(int argc, char *argv[])
{
	t_data	*d;

	d = init_s_data(d);
	init_s_arg(d, &d->arg, argc, argv);
	init_s_philo(d, &d->arg);
	init_mutex(d, &d->arg);
	start_dine(d, &d->arg, d->philo);
	philo_exit(d, NULL);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q philo");
}
