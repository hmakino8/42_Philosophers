/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 02:09:13 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/18 14:06:31 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philo.h"

void	ft_sleep(size_t	sleep)
{
	size_t	start;

	start = get_time();
	while (get_time() - start < sleep)
		usleep(10);
}

void	free_all_struct(t_philo_info *info)
{
	free(info->philo);
	free(info->forks);
	free(info);
}

void	destroy_mutex(t_philo_info *info)
{
	int	i;

	i = 0;
	while (i < info->arg.cnt_philo)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->monitor_finish);
	pthread_mutex_destroy(&info->monitor_output);
}

void	philo_exit(t_philo_info *info, char *error_msg)
{
	if (info != NULL)
	{
		destroy_mutex(info);
		free_all_struct(info);
	}
	if (error_msg)
	{
		printf("%s\n", error_msg);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	init_s_arg(t_philo_info *info, int argc, char *argv[])
{
	t_arg	*arg;

	arg = &info->arg;
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
		philo_exit(info, "Invalid argument.");
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	init_s_philo(t_philo_info *info)
{
	int	i;
	int	cnt_philo;

	cnt_philo = info->arg.cnt_philo;
	info->philo = malloc(sizeof(t_philo) * cnt_philo);
	if (info->philo == NULL)
		philo_exit(info, "Malloc failure.");
	i = 0;
	while (i < cnt_philo)
	{
		info->philo[i].id = i;
		info->philo[i].cnt_eat = 0;
		info->philo[i].fork_l = i;
		info->philo[i].fork_r = (i + 1) % cnt_philo;
		info->philo[i].info = info;
		info->philo[i].time_last_eat = get_time();
		pthread_mutex_init(&info->philo[i].monitor_eat, NULL);
		i++;
	}
}

void	init_s_philo_info(t_philo_info **info)
{
	*info = malloc(sizeof(t_philo_info));
	if (*info == NULL)
		philo_exit(*info, "Malloc failure.");
	(*info)->cnt_full = 0;
	(*info)->philo_status = 0;
	(*info)->finish = false;
}

void	put_msg(t_philo_info *info, t_philo *philo, int sig)
{
	int		id;
	size_t	time;

	pthread_mutex_lock(&info->monitor_output);
	id = philo->id + 1;
	time = get_time() - info->time_start;
	if (sig == DEAD)
		printf("%10zdms =>   philo_%-4d is died\n", time, id);
	else if (!info->finish)
	{
		if (sig == FORK)
			printf("%10zdms =>   philo_%-4d has taken a fork\n", time, id);
		if (sig == EAT)
			printf("%10zdms =>   philo_%-4d is eating\n", time, id);
		if (sig == SLEEP)
			printf("%10zdms =>   philo_%-4d is sleeping\n", time, id);
		if (sig == THINK)
			printf("%10zdms =>   philo_%-4d is thinking\n", time, id);
		if (sig == FULL)
			printf("%55s", " All philosophers are full\n");
	}
	pthread_mutex_unlock(&info->monitor_output);
}

void	philo_dine(t_philo_info *info, t_philo *philo)
{
	pthread_mutex_lock(&info->forks[philo->fork_r]);
	pthread_mutex_lock(&info->forks[philo->fork_l]);
	put_msg(info, philo, FORK);
	put_msg(info, philo, FORK);
	pthread_mutex_lock(&philo->monitor_eat);
	put_msg(info, philo, EAT);
	philo->time_last_eat = get_time();
	philo->cnt_eat++;
	pthread_mutex_unlock(&philo->monitor_eat);
	ft_sleep(info->arg.time_to_eat);
	pthread_mutex_unlock(&info->forks[philo->fork_r]);
	pthread_mutex_unlock(&info->forks[philo->fork_l]);
}

void	active_monitor_finish(t_philo_info *info, int status)
{
	put_msg(info, info->philo, status);
	pthread_mutex_lock(&info->monitor_finish);
	info->finish = true;
	pthread_mutex_unlock(&info->monitor_finish);
}

bool	is_full(t_philo_info *info, t_philo *philo, t_arg arg)
{
	if (arg.cnt_must_eat > 0 && philo->cnt_eat >= arg.cnt_must_eat)
	{
		pthread_mutex_lock(&philo->monitor_eat);
		info->cnt_full++;
		pthread_mutex_unlock(&philo->monitor_eat);
		if (info->cnt_full == info->arg.cnt_philo)
		{
			put_msg(info, info->philo, FULL);
			pthread_mutex_lock(&info->monitor_finish);
			info->finish = true;
			pthread_mutex_unlock(&info->monitor_finish);
		}
		if (info->cnt_full >= info->arg.cnt_philo)
			return (true);
	}
	return (false);
}

void	*routine(void *vptr)
{
	t_philo			*philo;
	t_philo_info	*info;
	t_arg			arg;

	philo = (t_philo *)vptr;
	info = philo->info;
	arg = info->arg;
	if (philo->id % 2)
		ft_sleep(10);
	philo->time_last_eat = get_time();
	while (!info->finish)
	{
		philo_dine(philo->info, philo);
		if (is_full(info, philo, arg))
			break ;
		put_msg(info, philo, SLEEP);
		ft_sleep(arg.time_to_sleep);
		put_msg(info, philo, THINK);
	}
	return (NULL);
}

bool	is_dead(t_philo_info *info)
{
	t_arg	arg;
	t_philo	*philo;

	arg = info->arg;
	philo = info->philo;
	return (get_time() - philo->time_last_eat > arg.time_to_die);
}

void	*confirmation_survival(void *vptr)
{
	t_philo_info	*info;

	info = (t_philo_info *)vptr;
	while (!info->finish)
	{
		if (is_dead(info))
		{
			put_msg(info, info->philo, DEAD);
			pthread_mutex_lock(&info->monitor_finish);
			info->finish = true;
			pthread_mutex_unlock(&info->monitor_finish);
			break ;
		}
		ft_sleep(1);
	}
	return (NULL);
}

void	start_dine(t_philo_info *info, t_philo *philo)
{
	int			i;
	int			cnt_philo;
	pthread_t	tid;

	cnt_philo = info->arg.cnt_philo;
	info->time_start = get_time();
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, routine, &philo[i]) != 0)
			philo_exit(info, "Failed to create pthread.");
		i++;
	}
	if (pthread_create(&tid, NULL, confirmation_survival, info) != 0)
		philo_exit(info, "Failed to create pthread.");
	if (pthread_detach(tid) != 0)
		philo_exit(info, "Failed to detach pthread.");
	i = 0;
	while (i < cnt_philo)
	{
		if (pthread_join(philo[i].tid, NULL) != 0)
			philo_exit(info, "Failed to join pthread.");
		i++;
	}
}

void	init_mutex(t_philo_info *info)
{
	int	i;
	int	cnt_philo;

	cnt_philo = info->arg.cnt_philo;
	info->forks = malloc(sizeof(pthread_mutex_t) * cnt_philo);
	if (info->forks == NULL)
		philo_exit(info, "Malloc failure.");
	i = 0;
	while (i < cnt_philo)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&info->monitor_finish, NULL);
	pthread_mutex_init(&info->monitor_output, NULL);
}

int	main(int argc, char *argv[])
{
	t_philo_info	*info;

	init_s_philo_info(&info);
	init_s_arg(info, argc, argv);
	init_s_philo(info);
	init_mutex(info);
	start_dine(info, info->philo);
	philo_exit(info, NULL);
}

//__attribute__((destructor)) static void destructor()
//{
//	system("leaks -q philo");
//}
