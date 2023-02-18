/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:34:39 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/19 01:31:01 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	pthread_mutex_destroy(&info->monitor_output);
}

void	philo_err_exit(char *error_msg)
{
	if (error_msg)
	{
		printf("%s\n", error_msg);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
