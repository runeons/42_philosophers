#include "philosophers.h"

int	check_input(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac < 5 || ac > 6)
	{
		printf(""R_B"Error:"W_B" input - wrong number of arguments\n"C_RES);
		return (-1);
	}
	while (av[++i])
	{
		if (ft_atoi_long(av[i]) > 0)
			continue ;
		else if (ft_atoi_long(av[i]) <= 0)
		{
			printf(""R_B"Error:"W_B" input - all arguments should be > 0\n"
				C_RES);
			return (-1);
		}
	}
	return (0);
}

void	init_phils(t_phil *phil, char**av, pthread_mutex_t *forks)
{
	int	i;
	int	starting_time;

	starting_time = get_time();
	i = -1;
	while (++i < ft_atoi_long(av[1]))
	{
		phil[i].starting_time = starting_time;
		init_phil(&phil[i], i, av, &forks);
	}
	return ;
}

int	main(int ac, char **av)
{
	t_phil			*phil;
	int				nb_phil;
	pthread_mutex_t	*forks;

	if (check_input(ac, av) == -1)
		return (-1);
	nb_phil = ft_atoi_long(av[1]);
	phil = (t_phil *)malloc(sizeof(t_phil) * (nb_phil + 1));
	forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (nb_phil + 1));
	pthread_mutex_init(&g_lock_print, NULL);
	init_phils(phil, av, forks);
	if (fork_mutexes(INIT, forks, nb_phil))
		return (-1);
	if (start_diner(phil, nb_phil))
		return (-1);
	pthread_mutex_destroy(&g_lock_print);
	if (fork_mutexes(DESTROY, forks, nb_phil))
		return (-1);
	free(phil);
	free(forks);
	return (0);
}
