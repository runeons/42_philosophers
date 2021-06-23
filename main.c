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

int		check_input(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf(""C_G_RED"Error:"C_G_WHITE" input - wrong number of arguments\n"C_RES);
		return (-1);
	}
	while (av[++i])
	{
		if (ft_atoi_long(av[i]) > 0)
			continue ;
		else if (ft_atoi_long(av[i]) <= 0)
		{
			printf(""C_G_RED"Error:"C_G_WHITE" input - all arguments should be > 0\n"C_RES);
			return (-1);
		}
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_shared	shared;
	t_phil		*phil;
	int			starting_time;
	int 		nb_phil;
	pthread_mutex_t	*forks;

	if (check_input(ac, av) == -1)
		return (-1);
	nb_phil = ft_atoi_long(av[1]);
	phil = (t_phil *)malloc(sizeof(t_phil) * (nb_phil + 1));
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (nb_phil + 1));
	int i;
	i = -1;
	starting_time = get_time();
	while (++i < nb_phil)
	{
		init_phil(starting_time, &phil[i], i, av, &forks);
		// phil[i].forks = forks;
	}
	pthread_mutex_init(&mutex, NULL);
	if (fork_mutexes(INIT, forks, nb_phil))
		return (-1);
	if (start_diner(&shared, phil, nb_phil))
		return (-1);
	pthread_mutex_destroy(&mutex);
	if (fork_mutexes(DESTROY, forks, nb_phil))
		return (-1);
	return (0);
}