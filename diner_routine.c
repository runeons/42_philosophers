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
		millisleep(curr->time_to_eat, curr->current_time, curr->starting_time);
		pthread_mutex_unlock(curr->fork_left);
		pthread_mutex_unlock(curr->fork_right);

		change_state_and_print(&curr, SLEEPING);
		millisleep(curr->time_to_sleep, curr->current_time, curr->starting_time);

		if (g_end)
			return (phil);
	}
	return (phil);
}

int		start_diner(t_phil *phils, int nb_phil)
{
	pthread_t	th_phil[nb_phil];
	void		*phil;
	int			i;

	i = -1;
	while (++i < nb_phil)
	{
		phil = (void *) &phils[i];
		if (pthread_create(&th_phil[i], NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	i = -1;
	while (++i < nb_phil)
	{
		phil = (void**) &phils[i];
		if (pthread_join(th_phil[i], phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	return (0);
}
