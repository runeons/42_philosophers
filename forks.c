#include "philosophers.h"

int	fork_mutexes(int option, pthread_mutex_t *forks, int nb_phil)
{
	int	i;

	i = -1;
	while (++i < nb_phil)
	{
		if (option == INIT)
			pthread_mutex_init(&forks[i], NULL);
		else if (option == DESTROY)
			pthread_mutex_destroy(&forks[i]);
		else
			return (print_error("invalid option mutexes handling", NULL));
	}
	return (0);
}
