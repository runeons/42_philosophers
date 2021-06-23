#include "philosophers.h"

void	*routine(void *phil)
{
	t_phil	*curr;

	curr = ((t_phil *)phil);
	if (curr->nb_phil == 1)
	{
		change_state_and_print(&curr, DIED);
		return (phil);
	}
	while (1)
	{
		change_state_and_print(&curr, THINKING);
		if (take_forks(&curr, curr->nb_phil, curr->id) == -1)
			return (phil);
		change_state_and_print(&curr, EATING);
		millisleep(curr->time_to_eat, curr->curr_time, curr->starting_time);
		if (curr->eating_times > 0)
			curr->eating_times--;
		pthread_mutex_unlock(curr->fork_left);
		pthread_mutex_unlock(curr->fork_right);
		change_state_and_print(&curr, SLEEPING);
		millisleep(curr->time_to_sleep, curr->curr_time, curr->starting_time);
		if (g_end || curr->eating_times == 0)
			return (phil);
	}
	return (phil);
}

int	start_diner(t_phil *phils, int nb_phil)
{
	void	*phil;
	int		i;

	i = -1;
	while (++i < nb_phil)
	{
		phil = (void *) &phils[i];
		if (pthread_create(&(phils[i].th_phil), NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	i = -1;
	while (++i < nb_phil)
	{
		phil = (void **) &phils[i];
		if (pthread_join(phils[i].th_phil, phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	return (0);
}
