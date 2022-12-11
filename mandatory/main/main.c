/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2022/12/11 23:26:55 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

void	free_all_struct(t_data *d)
{
	free(d->philo);
	free(d->forks);
	free(d);
}

void	destroy_mutex(t_data *d, int cnt_philo)
{
	int	i;

	i = -1;
	while (++i < cnt_philo)
		pthread_mutex_destroy(&d->forks[i]);
	pthread_mutex_destroy(&d->monitor_finish);
	pthread_mutex_destroy(&d->monitor_output);
}

void	philo_exit(t_data *d, char *error_msg)
{
	if (d)
	{
		destroy_mutex(d, d->arg.cnt_philo);
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
		if (pthread_mutex_init(&d->philo[i].monitor_eat, NULL) != SUCCESS)
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

void	put_status_message(t_philo *philo, char *msg)
{
	int			id;
	int			ms;
	long int	sec;

	id = philo->id + 1;
	ms = philo->time_last_eat.ms;
	sec = philo->time_last_eat.s;
	printf("%ld%d %d %s\n", sec, ms, id, msg);
}

void	philo_dine(t_data *d, t_philo *philo)
{
	pthread_mutex_lock(&d->forks[philo->fork_r]);
	put_status_message(philo, "has taken a fork");
	pthread_mutex_lock(&d->forks[philo->fork_l]);
	put_status_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->monitor_eat);
	get_time(&philo->time_last_eat);
	put_status_message(philo, "is eating");
	pthread_mutex_unlock(&philo->monitor_eat);
	usleep(d->arg.time_to_eat);
	pthread_mutex_unlock(&d->forks[philo->fork_r]);
	pthread_mutex_unlock(&d->forks[philo->fork_l]);
}

void	*routine(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (philo->id % 2)
		usleep(200);
	get_time(&philo->time_last_eat);
	while (philo->status != FULL && philo->status != DEAD)
	{
		philo_dine(philo->d, philo);
	}
	return (NULL);
}

void	*confirmation_survival(void *p)
{
	return (NULL);
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
	pthread_mutex_t *pid;

	pthread_mutex_init(pid,NULL);
	pthread_mutex_destroy(pid);
		//if (pthread_mutex_init(pid, NULL) != SUCCESS)
		//	philo_exit(d, "NULLNULL.");
	//d->forks = ft_calloc(sizeof(pthread_mutex_t), arg->cnt_philo);
	//if (!d->forks)
	//	philo_exit(d, "Malloc failure.");
	//i = -1;
	//while (++i < arg->cnt_philo)
	//{
	//	if (pthread_mutex_init(&d->forks[i], NULL) != SUCCESS)
	//		philo_exit(d, "Failed to init mutex.");
	//}
	//if (pthread_mutex_init(&d->monitor_finish, NULL) != SUCCESS)
	//	philo_exit(d, "Failed to init mutex.");
	//if (pthread_mutex_init(&d->monitor_output, NULL) != SUCCESS)
	//	philo_exit(d, "Failed to init mutex.");
}

int	main(int argc, char *argv[])
{
	t_data	*d;

	d = init_s_data(d);
	init_s_arg(d, &d->arg, argc, argv);
	init_s_philo(d, &d->arg);
	init_mutex(d, &d->arg);
	//start_dine(d, &d->arg, d->philo);
	//philo_exit(d, NULL);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q philo");
}
