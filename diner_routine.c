#include "philosophers.h"

void	*monitor(void *phil)
{
	t_phil	*curr;

	curr = ((t_phil *)phil);
	while (1)
	{
		if (curr->eating_times == 0)
			return (phil);
		if (ret_current_time(*curr) - curr->last_eating > curr->time_to_die)
		{
			change_state_and_print(&curr, DIED);
			pthread_mutex_unlock(curr->fork_left);
			pthread_mutex_unlock(curr->fork_right);
			return (phil);
		}
		if (*curr->end)
			return (phil);
	}
	return (phil);
}

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
		// thinks
		change_state_and_print(&curr, THINKING);
		// take forks
		if (take_forks(&curr, curr->nb_phil, curr->id) == -1)
			return (phil);
		// eats
		change_state_and_print(&curr, EATING);
		millisleep(curr->time_to_eat, curr->curr_time, curr->starting_time, curr->end);
		if (curr->eating_times > 0)
			curr->eating_times--;
		// release forks
		pthread_mutex_unlock(curr->fork_left);
		pthread_mutex_unlock(curr->fork_right);
		// sleeps
		change_state_and_print(&curr, SLEEPING);
		millisleep(curr->time_to_sleep, curr->curr_time, curr->starting_time, curr->end);
		if (*curr->end || curr->eating_times == 0)
			return (phil);
	}
	return (phil);
}

int	start_diner(t_phil *phils, int nb_phil)
{
	void	*phil;
	int		i;
	int		end;

	end = 0;
	i = -1;
	while (++i < nb_phil)
	{
		phils[i].end = &end;
		phil = (void *) &phils[i];
		if (pthread_create(&(phils[i].th_phil), NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
		if (pthread_create(&(phils[i].th_monitor), NULL, &monitor, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	i = -1;
	while (++i < nb_phil)
	{
		phil = (void **) &phils[i];
		if (pthread_join(phils[i].th_phil, phil) != 0)
			return (print_error("Failed to join thread", NULL));
		if (pthread_join(phils[i].th_monitor, phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	return (0);
}
