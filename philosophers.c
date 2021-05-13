#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "color_shell.h"

// none dies
// # define NB_PHIL 4
// # define TIME_TO_DIE 410
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 200

// // one dies
// # define NB_PHIL 4
// # define TIME_TO_DIE 310
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 100
//
// none dies
// # define NB_PHIL 5
// # define TIME_TO_DIE 800
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 200
//
# define NB_PHIL 7
# define TIME_TO_DIE 310
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 200
//
# define THINKING 0
# define TAKEN_A_FORK 1
# define EATING 2
# define SLEEPING 3
# define DIED 4

# define INIT 0
# define DESTROY 1

# define LEFT 0
# define RIGHT 1

/*
gcc -Wall -Wextra -Werror philosophers.c && ~/valgrind_fork/bin/valgrind ./a.out
*/

int	g_end = 0;
pthread_mutex_t mutex;

typedef struct		s_phil
{
	int				nb_phil;
	int				id;
	int				eating_times;
	int				is_thinking;
	int				has_taken_a_fork;
	int				is_eating;
	int				is_sleeping;
	int				died;
	int				left;
	int				right;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	*lock_print;
	int				starting_time;
	int				current_time;
	int				last_eating;
}					t_phil;

typedef struct		s_shared
{
	pthread_mutex_t	forks[NB_PHIL];
	pthread_mutex_t	lock_print;
	t_phil			phil[NB_PHIL];
	int				nb_phil;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	int				starting_time;
}					t_shared;

void	init_phil(t_shared *shared, t_phil *phil, int id)
{
	phil->nb_phil = shared->nb_phil;
	phil->id = id;
	phil->is_thinking = 0;
	phil->has_taken_a_fork = 0;
	phil->is_eating = 0;
	phil->is_sleeping = 0;
	phil->died = 0;
	phil->eating_times = 0;
	phil->left = id - 1;
	if (id == 0)
		phil->left = NB_PHIL;
	phil->right = phil->id;
	phil->fork_left = &shared->forks[phil->left];
	phil->fork_right = &shared->forks[phil->id];
	phil->lock_print = &shared->lock_print;
	phil->starting_time = shared->starting_time;
	phil->current_time = 0;
	phil->last_eating = 0;
}

int    get_time(void)
{
    struct timeval	tp;
    int				milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

void	init_shared(t_shared *shared)
{
	shared->nb_phil = NB_PHIL;
	shared->time_to_eat = TIME_TO_EAT;
	shared->time_to_sleep = TIME_TO_SLEEP;
	shared->time_to_die = TIME_TO_DIE;
	shared->starting_time = get_time();
}

int		print_error(char *msg, t_phil *phil)
{
	if (phil)
		printf(C_G_RED"%d %s"C_RES"\n", phil->id, msg);
	else
		printf(C_G_RED"%s"C_RES"\n", msg);
	return (-1);
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

int		ret_current_time(t_phil phil)
{
	return (get_time() - phil.starting_time);
}

void	print(t_phil *phil, int option)
{
	if (option == DIED && g_end == 0)
	{
		g_end = 1;
		printf("|| %7i ms || %7i ms || %-16s || %-21s || %-13s || %-16s || %3i%-5s ||\n", phil->current_time , ret_current_time(*phil),
		"", "", "", "", phil->id, " died");
		millisleep(100, phil->current_time, phil->starting_time);
	}
	else if (g_end)
		return ;
	else if (option == THINKING)
		printf("|| %7i ms || %7i ms || %3i%-*s || %-*s || %-*s || %-*s || %-*s ||\n", phil->current_time , ret_current_time(*phil), phil->id, 13,
		" is thinking", 21, "", 13, "", 16, "", 8, "");
	else if (option == TAKEN_A_FORK)
		printf("|| %7i ms || %7i ms || %-16s || %3i%-18s || %-13s || %-16s || %-8s ||\n", phil->current_time , ret_current_time(*phil), "",
		phil->id, " has taken a fork", "", "", "");
	else if (option == EATING)
		printf("|| %7i ms || %7i ms || %-16s || %-21s || %3i%-10s || %-16s || %-8s ||\n", phil->current_time , ret_current_time(*phil),
		"", "", phil->id, " is eating","","");
	else if (option == SLEEPING)
		printf("|| %7i ms || %7i ms || %-16s || %-21s || %-13s || %3i%-13s || %-8s ||\n", phil->current_time , ret_current_time(*phil),
		"", "", "", phil->id, " is sleeping", "");
}

void	print_rendu(t_phil *phil, int option)
{
	if (option == DIED && g_end == 0)
	{
		g_end = 1;
		printf("%8i %3i %s\n", phil->current_time, phil->id, " died");
		millisleep(100, phil->current_time, phil->starting_time);
	}
	else if (g_end)
		return ;
	else if (option == THINKING)
		printf("%8i %3i %s\n", phil->current_time, phil->id, " is thinking");
	else if (option == TAKEN_A_FORK)
		printf("%8i %3i %s\n", phil->current_time, phil->id, " has taken a fork");
	else if (option == EATING)
		printf("%8i %3i %s\n", phil->current_time, phil->id, " is eating");
	else if (option == SLEEPING)
		printf("%8i %3i %s\n", phil->current_time, phil->id, " is sleeping");

}

int		change_state_and_print(t_phil **phil, int new_state)
{
	(*phil)->current_time = ret_current_time(**phil);
	if (new_state == THINKING)
	{
		(*phil)->is_thinking = 1;
		if ((*phil)->is_eating == 1)
			return (print_error("was not supposed to eat", (*phil)));
		(*phil)->is_sleeping = 0;
	}
	else if (new_state == EATING)
	{
		(*phil)->last_eating = (*phil)->current_time;
		(*phil)->is_thinking = 0;
		(*phil)->is_eating = 1;
		if ((*phil)->is_sleeping == 1)
			return (print_error("was not supposed to sleep", (*phil)));
	}
	else if (new_state == SLEEPING)
	{
		if ((*phil)->is_thinking == 1)
			return (print_error("was not supposed to think", (*phil)));
		(*phil)->is_eating = 0;
		(*phil)->is_sleeping = 0;
	}
	else if (new_state == DIED)
	{
		// g_end = 1;
		(*phil)->is_thinking = 0;
		(*phil)->is_eating = 0;
		(*phil)->is_sleeping = 0;
		(*phil)->died = 1;
	}
	else if (new_state == TAKEN_A_FORK)
		;
	else
		return (print_error("undefined new_state", (*phil)));
	// pthread_mutex_lock((*phil)->lock_print);
	pthread_mutex_lock(&mutex);
	// print((*phil), new_state);
	print_rendu((*phil), new_state);
	pthread_mutex_unlock(&mutex);
	// pthread_mutex_unlock((*phil)->lock_print);
	return (0);
}

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

int		take_fork(t_phil **phil, int option)
{
	if (ret_current_time(**phil) - (*phil)->last_eating > TIME_TO_DIE)
	{
		change_state_and_print(phil, DIED);
		if (option == LEFT)
			pthread_mutex_unlock((*phil)->fork_left);
		else if (option == RIGHT)
			pthread_mutex_unlock((*phil)->fork_right);
		return (-1);
	}
	else if (option == LEFT)
		pthread_mutex_lock((*phil)->fork_left);
	else if (option == RIGHT)
		pthread_mutex_lock((*phil)->fork_right);
	else
		print_error("invalid fork option", *phil);
	change_state_and_print(phil, TAKEN_A_FORK);

	return (0);
}

int		take_forks(t_phil **phil, int nb_phil, int id)
{
	if (nb_phil % 2)
	{
		if (id % 2)
		{
			if (take_fork(phil, RIGHT) == -1)
				return (-1);
			if (take_fork(phil, LEFT) == -1)
				return (-1);
		}
		else
		{
			if (take_fork(phil, LEFT) == -1)
				return (-1);
			if (take_fork(phil, RIGHT) == -1)
				return (-1);
		}
	}
	else
	{
		if (take_fork(phil, LEFT) == -1)
			return (-1);
		if (take_fork(phil, RIGHT) == -1)
			return (-1);
	}
	return (0);
}

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

	printf("  ============  ============  ==================  =======================  ===============  ==================  ==========  \n");
	i = -1;
	while (++i < NB_PHIL)
	{
		init_phil(shared, &shared->phil[i], i);
		phil = (void *)&shared->phil[i];
		// printf(""C_G_RED"create %i beg"C_RES"\n", i);
		if (pthread_create(&th_phil[i], NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
		// c'est ici que je devrais verifier s'il est mort
	}
	i = -1;
	while (++i < NB_PHIL)
	{
		// printf(""C_G_RED"join %i"C_RES"\n", i);
		phil = (void**) &shared->phil[i];
		if (pthread_join(th_phil[i], phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	printf("  ============  ============  ==================  =======================  ===============  ==================  ==========  \n");
	return (0);
}

int		fork_mutexes(int option, t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < NB_PHIL)
	{
		if (option == INIT)
			pthread_mutex_init(&shared->forks[i], NULL);
		else if (option == DESTROY)
			pthread_mutex_destroy(&shared->forks[i]);
		else
			return (print_error("invalid option mutexes handling", NULL));
	}
	return (0);
}

long	ft_atoi_long(char const *s)
{
	int		i;
	int		n;
	long	res;

	i = 0;
	res = 0;
	n = 1;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			n = n * -1;
		i++;
	}
	while (s[i] >= 48 && s[i] <= 57)
	{
		res = res * 10 + (s[i] - 48);
		i++;
	}
	return (res * n);
}

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