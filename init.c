#include "philosophers.h"

void	init_phil(t_phil *phil, int id, char**av, pthread_mutex_t **forks)
{
	phil->nb_phil = ft_atoi_long(av[1]);
	phil->time_to_die = ft_atoi_long(av[2]);
	phil->time_to_eat = ft_atoi_long(av[3]);
	phil->time_to_sleep = ft_atoi_long(av[4]);
	phil->id = id;
	phil->died = 0;
	phil->eating_times = -1;
	if (av[5] && ft_atoi_long(av[5]) > 0)
		phil->eating_times = ft_atoi_long(av[5]);
	phil->left = id - 1;
	if (id == 0)
		phil->left = phil->nb_phil - 1;
	phil->right = phil->id;
	phil->forks = *forks;
	phil->fork_left = &phil->forks[phil->left];
	phil->fork_right = &phil->forks[phil->id];
	phil->curr_time = 0;
	phil->last_eating = 0;
	phil->end = NULL;
	// pthread_mutex_init(&phil->die_and_eat, NULL);
}
