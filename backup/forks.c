#include "philosophers.h"

int		take_fork(t_phil **phil, int option)
{
	if (ret_current_time(**phil) - (*phil)->last_eating > TIME_TO_DIE)
	{
		change_state_and_print(phil, DIED);
		if (option == LEFT)
			pthread_mutex_unlock((*phil)->fork_left);
		else if (option == RIGHT)
			pthread_mutex_unlock((*phil)->fork_right);
		return (-1);
	}
	else if (option == LEFT)
		pthread_mutex_lock((*phil)->fork_left);
	else if (option == RIGHT)
		pthread_mutex_lock((*phil)->fork_right);
	else
		print_error("invalid fork option", *phil);
	change_state_and_print(phil, TAKEN_A_FORK);

	return (0);
}

int		take_forks(t_phil **phil, int nb_phil, int id)
{
	if (nb_phil % 2)
	{
		if (id % 2)
		{
			if (take_fork(phil, RIGHT) == -1)
				return (-1);
			if (take_fork(phil, LEFT) == -1)
				return (-1);
		}
		else
		{
			if (take_fork(phil, LEFT) == -1)
				return (-1);
			if (take_fork(phil, RIGHT) == -1)
				return (-1);
		}
	}
	else
	{
		if (take_fork(phil, LEFT) == -1)
			return (-1);
		if (take_fork(phil, RIGHT) == -1)
			return (-1);
	}
	return (0);
}

int		fork_mutexes(int option, t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < NB_PHIL)
	{
		if (option == INIT)
			pthread_mutex_init(&shared->forks[i], NULL);
		else if (option == DESTROY)
			pthread_mutex_destroy(&shared->forks[i]);
		else
			return (print_error("invalid option mutexes handling", NULL));
	}
	return (0);
}
