#include "philosophers.h"

void	init_phil(int starting_time, t_phil *phil, int id, char**av, pthread_mutex_t **forks)
{
	phil->nb_phil = ft_atoi_long(av[1]);
	phil->time_to_die = ft_atoi_long(av[2]);
	phil->time_to_eat = ft_atoi_long(av[3]);
	phil->time_to_sleep = ft_atoi_long(av[4]);
	phil->starting_time = starting_time;
	phil->id = id;
	phil->is_thinking = 0;
	phil->has_taken_a_fork = 0;
	phil->is_eating = 0;
	phil->is_sleeping = 0;
	phil->died = 0;
	phil->eating_times = 0;
	phil->left = id - 1;
	if (id == 0)
		phil->left = phil->nb_phil;
	phil->right = phil->id;
	phil->forks = *forks;
	phil->fork_left = &phil->forks[phil->left];
	phil->fork_right = &phil->forks[phil->id];
	phil->current_time = 0;
	phil->last_eating = 0;
}
