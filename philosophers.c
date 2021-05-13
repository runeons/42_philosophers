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
# define THINKING 7
# define EATING 8
# define SLEEPING 9

/*
gcc -Wall -Wextra -Werror philosophers.c && ~/valgrind_fork/bin/valgrind ./a.out
*/

pthread_mutex_t	forks[NB_FORK];

typedef struct		s_phil
{
	int			id;
	int			eating_times;
	int			is_thinking;
	int			is_eating;
	int			is_sleeping;
	int			died;
	int			left;
	int			right;
}					t_phil;

typedef struct		s_shared
{
	t_phil		phil[NB_PHIL];
	int			nb_phil;
	int			time_to_eat;
	int			time_to_sleep;
	int			time_to_die;
}					t_shared;

void	init_phil(t_phil *phil, int id)
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

int		change_state_and_print(t_phil *phil, int new_state)
{
	if (new_state == THINKING)
	{
		if (phil->is_eating == 1)
			return (print_error("was not supposed to eat", phil));
		phil->is_sleeping = 0;
		phil->is_thinking = 1;
		printf(""C_YELLOW"%i is thinking"C_RES"\n", phil->id);
	}
	else if (new_state == EATING)
	{
		if (phil->is_sleeping == 1)
			return (print_error("was not supposed to sleep", phil));
		phil->is_thinking = 0;
		phil->is_eating = 1;
		printf(""C_GREEN"%i is eating"C_RES"\n", phil->id);
	}
	else if (new_state == SLEEPING)
	{
		if (phil->is_thinking == 1)
			return (print_error("was not supposed to think", phil));
		phil->is_eating = 0;
		phil->is_sleeping = 0;
		printf(""C_CYAN"%i is sleeping"C_RES"\n", phil->id);
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
	pthread_mutex_lock(&forks[curr->left]);
	pthread_mutex_lock(&forks[curr->right]);

	change_state_and_print(curr, EATING);
	usleep(TIME_TO_EAT);
	curr->eating_times++;
	printf(""C_BLUE"%i has eaten"C_RES"\n", curr->id);
	pthread_mutex_unlock(&forks[curr->left]);
	pthread_mutex_unlock(&forks[curr->right]);

	change_state_and_print(curr, SLEEPING);
	usleep(TIME_TO_SLEEP);
	printf(""C_BLUE"%i has slept"C_RES"\n", curr->id);
	return (phil);
}

int		start_diner(t_shared *shared)
{
	pthread_t	th_phil[NB_PHIL];
	void		*phil;
	int			i;

	i = -1;
	while (++i < NB_PHIL)
	{
		init_phil(&shared->phil[i], i);
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
	return (0);
}

int		main(int ac, char **av)
{
	int				i;
	t_shared		shared;

	(void)ac;
	(void)av;
	init_shared(&shared);
// creer les forks
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_init(&forks[i], NULL);
	if (start_diner(&shared))
		return (-1);
// detruire les forks
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_destroy(&forks[i]);
	return (0);
}