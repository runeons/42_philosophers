#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "color_shell.h"

# define NB_FORK 5
# define NB_PHIL 5
# define TIME_TO_EAT 200000
# define TIME_TO_SLEEP 300000
# define TIME_TO_DIE 400000
# define THINKING 0
# define EATING 1
# define SLEEPING 2
# define DYING 3
# define INIT 0
# define DESTROY 1

/*
gcc -Wall -Wextra -Werror philosophers.c && ~/valgrind_fork/bin/valgrind ./a.out
*/

typedef struct		s_phil
{
	int				id;
	int				eating_times;
	int				is_thinking;
	int				is_eating;
	int				is_sleeping;
	int				died;
	int				left;
	int				right;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
}					t_phil;

typedef struct		s_shared
{
	pthread_mutex_t	forks[NB_FORK];
	t_phil			phil[NB_PHIL];
	int				nb_phil;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
}					t_shared;

void	init_phil(t_shared *shared, t_phil *phil, int id)
{
	phil->id = id;
	phil->is_thinking = 0;
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
}

void	init_shared(t_shared *shared)
{
	// t_phil		phil[NB_PHIL];
	shared->nb_phil = NB_PHIL;
	shared->time_to_eat = TIME_TO_EAT;
	shared->time_to_sleep = TIME_TO_SLEEP;
	shared->time_to_die = TIME_TO_DIE;
}

int		print_error(char *msg, t_phil *phil)
{
	if (phil)
		dprintf(2, C_G_RED"%d %s"C_RES"\n", phil->id, msg);
	else
		dprintf(2, C_G_RED"%s"C_RES"\n", msg);
	return (-1);
}

/*
printf("|| %3i%-13s || %3i%-18s || %3i%-10s || %3i%-13s || %3i%-5s ||\n",
	phil->id, " is thinking", phil->id, " has taken a fork", phil->id,
	" is eating", phil->id, " is sleeping", phil->id, " died");

	printf("|| %-16s || %-21s || %-13s || %-16s || %-8s ||\n",
		"", "", "", "", "");

*/

int		change_state_and_print(t_phil *phil, int new_state)
{
	if (new_state == THINKING)
	{
		phil->is_thinking = 1;
		if (phil->is_eating == 1)
			return (print_error("was not supposed to eat", phil));
		phil->is_sleeping = 0;
		printf("|| %3i%-13s || %-21s || %-13s || %-16s || %-8s ||\n",
			phil->id, " is thinking", "", "", "", "");
	}
	else if (new_state == EATING)
	{
		phil->is_thinking = 0;
		phil->is_eating = 1;
		if (phil->is_sleeping == 1)
			return (print_error("was not supposed to sleep", phil));
		printf("|| %-16s || %-21s || %3i%-10s || %-16s || %-8s ||\n",
			"", "", phil->id,
			" is eating","","");
	}
	else if (new_state == SLEEPING)
	{
		if (phil->is_thinking == 1)
			return (print_error("was not supposed to think", phil));
		phil->is_eating = 0;
		phil->is_sleeping = 0;
		printf("|| %-16s || %-21s || %-13s || %3i%-13s || %-8s ||\n",
			"", "", "", phil->id, " is sleeping", "");
	}
	else if (new_state == DYING)
	{
		phil->is_thinking = 0;
		phil->is_eating = 0;
		phil->is_sleeping = 0;
		phil->died = 0;
		printf("|| %-16s || %-21s || %-13s || %-16s || %3i%-5s ||\n",
		"", "", "", "", phil->id, " died");
	}
	else
		return (print_error("undefined new_state", phil));
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

void	*routine(void *phil)
{
	t_phil	*curr;

	curr = ((t_phil *)phil);
	change_state_and_print(curr, THINKING);
	pthread_mutex_lock(curr->fork_left);
	printf("|| %-16s || %3i%-18s || %-13s || %-16s || %-8s ||\n", "",
		curr->id, " has taken a fork", "", "", "");
	pthread_mutex_lock(curr->fork_right);
	printf("|| %-16s || %3i%-18s || %-13s || %-16s || %-8s ||\n", "",
		curr->id, " has taken a fork", "", "", "");
	// pthread_mutex_lock(&forks[curr->left]);
	// pthread_mutex_lock(&forks[curr->right]);

	change_state_and_print(curr, EATING);
	usleep(TIME_TO_EAT);
	curr->eating_times++;
	// printf(""C_BLUE"%3i has eaten"C_RES"\n", curr->id);
	pthread_mutex_unlock(curr->fork_left);
	pthread_mutex_unlock(curr->fork_right);

	change_state_and_print(curr, SLEEPING);
	usleep(TIME_TO_SLEEP);
	// printf(""C_BLUE"%3i has slept"C_RES"\n", curr->id);
	return (phil);
}

int		start_diner(t_shared *shared)
{
	pthread_t	th_phil[NB_PHIL];
	void		*phil;
	int			i;

	printf("  ==================  =======================  ===============  ==================  ==========  \n");
	i = -1;
	while (++i < NB_PHIL)
	{
		init_phil(shared, &shared->phil[i], i);
		phil = (void *)&shared->phil[i];
		if (pthread_create(&th_phil[i], NULL, &routine, phil) != 0)
			return (print_error("Failed to create thread", NULL));
	}
	i = -1;
	while (++i < NB_PHIL)
	{
		phil = (void**) &shared->phil[i];
		if (pthread_join(th_phil[i], phil) != 0)
			return (print_error("Failed to join thread", NULL));
	}
	printf("  ==================  =======================  ===============  ==================  ==========  \n");
	return (0);
}

int		fork_mutexes(int option, t_shared *shared)
{
	int	i;

	i = -1;
	while (++i < NB_FORK)
	{
		if (option == INIT)
			pthread_mutex_init(&shared->forks[i], NULL);
		else if (option == DESTROY)
			pthread_mutex_destroy(&shared->forks[i]);
		else
			return (print_error("invalid option in fork mutexes handling", NULL));
	}
	return (0);
}

int		main(int ac, char **av)
{
	t_shared	shared;

	(void)ac;
	(void)av;
	init_shared(&shared);
	if (fork_mutexes(INIT, &shared))
		return (-1);
	if (start_diner(&shared))
		return (-1);
	if (fork_mutexes(DESTROY, &shared))
		return (-1);
	return (0);
}