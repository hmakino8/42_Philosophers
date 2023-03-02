/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroaki <hiroaki@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 17:01:00 by hiroaki           #+#    #+#             */
/*   Updated: 2023/02/22 18:23:47 by hmakino          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_philo_info	info;

	if (init_s_philo_info(&info, argc, argv) || \
		start_dine(&info, info.philos))
		return (EXIT_FAILURE);
	destroy_mutex(&info);
	return (EXIT_SUCCESS);
}
