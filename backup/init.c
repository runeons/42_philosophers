#include "philosophers.h"

void	init_phil(t_shared *shared, t_phil *phil, int id)
{
	phil->nb_phil = shared->nb_phil;
	phil->id = id;
	phil->is_thinking = 0;
	phil->has_taken_a_fork = 0;
	phil->is_eating = 0;
	phil->is_sleeping = 0;
	phil->died = 0;
	phil->eating_times = 0;
	phil->left = id - 1;
	if (id == 0)
		phil->left = NB_PHIL;
	phil->right = phil->id;
	phil->fork_left = &shared->forks[phil->left];
	phil->fork_right = &shared->forks[phil->id];
	phil->lock_print = &shared->lock_print;
	phil->starting_time = shared->starting_time;
	phil->current_time = 0;
	phil->last_eating = 0;
}

void	init_shared(t_shared *shared)
{
	shared->nb_phil = NB_PHIL;
	shared->time_to_eat = TIME_TO_EAT;
	shared->time_to_sleep = TIME_TO_SLEEP;
	shared->time_to_die = TIME_TO_DIE;
	shared->starting_time = get_time();
}
