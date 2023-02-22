/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 21:34:39 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 18:22:38 by hiroaki          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_mutex(t_philo_info *info)
{
	int	i;

	i = 0;
	while (i < info->arg.cnt_philo)
	{
		pthread_mutex_destroy(&info->forks[i]);
		pthread_mutex_destroy(&info->philos[i].mtx_eat);
		i++;
	}
	pthread_mutex_destroy(&info->mtx_put);
	pthread_mutex_destroy(&info->mtx_fin);
}

int	philo_err_exit(char *error_msg)
{
	write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	return (EXIT_FAILURE);
}
