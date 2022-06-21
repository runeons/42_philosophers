#include "philosophers.h"

int	print_error(char *msg, t_phil *phil)
{
	if (phil)
		printf(C_G_RED"%d %s"C_RES"\n", phil->id, msg);
	else
		printf(C_G_RED"%s"C_RES"\n", msg);
	return (-1);
}

void	print_rendu(t_phil *phil, int option)
{
	if (option == DIED && *phil->end == 0)
	{
		*phil->end = 1;
		printf("%06i %i %s\n", phil->curr_time, phil->id, "died");
		millisleep(1, phil->curr_time, phil->starting_time, phil->end);
	}
	else if (*phil->end)
		return ;
	else if (option == THINKING)
		printf("%06i %i %s\n", phil->curr_time, phil->id, "is thinking");
	else if (option == TAKEN_A_FORK)
		printf("%06i %i %s\n", phil->curr_time, phil->id, "has taken a fork");
	else if (option == EATING)
		printf("%06i %i %s\n", phil->curr_time, phil->id, "is eating");
	else if (option == SLEEPING)
		printf("%06i %i %s\n", phil->curr_time, phil->id, "is sleeping");
}

int	change_state_and_print(t_phil **phil, int new_state)
{
	(*phil)->curr_time = ret_current_time(**phil);
	if (new_state == EATING)
		(*phil)->last_eating = (*phil)->curr_time;
	else if (new_state == DIED)
		(*phil)->died = 1;
	pthread_mutex_lock((*phil)->lock_print);
	print_rendu((*phil), new_state);
	pthread_mutex_unlock((*phil)->lock_print);
	return (0);
}
