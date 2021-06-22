#include "philosophers.h"

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
