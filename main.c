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