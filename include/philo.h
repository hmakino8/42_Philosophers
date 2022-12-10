#ifndef PHILO
# define PHILO

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include "utils.h"

typedef struct s_arg
{
	int		cnt_philo;
	int		cnt_must_eat;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
}	t_arg;

typedef	struct s_time
{
	long int	s;
	int			ms;
}	t_time;

typedef struct s_philo
{
	int				idx;
	int				cnt_eat;
	int				time_last_eat;
	bool			fork_l;
	bool			fork_r;
	pthread_t		thread;
	pthread_mutex_t	monitor_eat;
}	t_philo;

typedef struct s_data
{
	t_arg	arg;
	t_time	time_start;
	t_philo	*philo;
}	t_data;

#endif
