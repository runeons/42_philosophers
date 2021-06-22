#include "philosophers.h"

int g_end = 0;

/*
pthread_mutex_lock(&fork)
	<=> if (lock == 1)
	{
		// wait until lock == 0
	}
	lock = 1;
do whatever
pthread_mutex_unlock(&fork)
	<=> lock == 1
*/

/*
** return -1 if philosopher died
*/

void	*routine(void *phil)
{
	t_phil	*curr;

	curr = ((t_phil *)phil);
	while (1)
	{
		change_state_and_print(&curr, THINKING);

		if (take_forks(&curr, curr->nb_phil, curr->id) == -1)
			return (phil);

		change_state_and_print(&curr, EATING);
		millisleep(TIME_TO_EAT, curr->current_time, curr->starting_time);
		pthread_mutex_unlock(curr->fork_left);
		pthread_mutex_unlock(curr->fork_right);

		change_state_and_print(&curr, SLEEPING);
		millisleep(TIME_TO_SLEEP, curr->current_time, curr->starting_time);

		if (g_end)
			return (phil);
	}
	return (phil);
}

int		start_diner(t_shared *shared)
{
	pthread_t	th_phil[NB_PHIL];
	void		*phil;
	int			i;

	i = -1;
	while (++i < NB_PHIL)
	{
		init_phil(shared, &shared->phil[i], i);
		phil = (void *)&shared->phil[i];
		if (pthread_create(&th_phil[i], NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	i = -1;
	while (++i < NB_PHIL)
	{
		phil = (void**) &shared->phil[i];
		if (pthread_join(th_phil[i], phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	return (0);
}

int		check_av(char **av)
{
	int	i;

	i = 0;
	while (av[++i])
	{
		if (ft_atoi_long(av[i]) > 0)
			continue ;
		else if (ft_atoi_long(av[i]) <= 0)
			return (-1);
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_shared	shared;

	// if (ac < 5 || ac > 6)
	// 	return (-1);
	// if (check_av(av) == -1)
	// 	return (-1);
	(void)ac;
	(void)av;
	init_shared(&shared);
	pthread_mutex_init(&shared.lock_print, NULL);
	pthread_mutex_init(&mutex, NULL);
	if (fork_mutexes(INIT, &shared))
		return (-1);
	if (start_diner(&shared))
		return (-1);
	pthread_mutex_destroy(&shared.lock_print);
	pthread_mutex_destroy(&mutex);
	if (fork_mutexes(DESTROY, &shared))
		return (-1);
	return (0);
}