#include "philosophers.h"

int	get_time(void)
{
	struct timeval	tp;
	int				milliseconds;

	gettimeofday(&tp, NULL);
	milliseconds = tp.tv_sec * 1000;
	milliseconds += tp.tv_usec / 1000;
	return (milliseconds);
}

void	millisleep(int to_spend, int current_time, int starting_time)
{
	int	time_left;

	time_left = current_time + to_spend - (get_time() - starting_time);
	while (time_left > 0)
	{
		time_left = current_time + to_spend - (get_time() - starting_time);
		usleep(1 * 1000);
		time_left = current_time + to_spend - (get_time() - starting_time);
	}
}

int	ret_current_time(t_phil phil)
{
	return (get_time() - phil.starting_time);
}
