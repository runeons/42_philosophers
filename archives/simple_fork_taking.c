#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "color_shell.h"

# define NB_PHIL 5
# define NB_FORK 1
# define TIME_TO_EAT 200000

/*
** gcc simple_fork_taking.c && ~/valgrind_fork/bin/valgrind ./a.out
*/

pthread_mutex_t	fourchettes[NB_FORK];

int	availableForks = 2;

void	*routine(void *id)
{
    int	local_id;
	int	i;

	local_id = *(int *)id;
	i = -1;
	printf("in routine\n");
	while (++i < NB_FORK)
		pthread_mutex_lock(&fourchettes[i]);
	if (availableForks == 0)
		printf("No more forks... going home\n");
	else
	{
		availableForks -= 2;
		printf(""C_G_RED"%i is eating"C_RES"\n", local_id);
		usleep(TIME_TO_EAT);
		availableForks += 2;
		printf(""C_G_GREEN"%i has eaten"C_RES"\n", local_id);

	}
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_unlock(&fourchettes[i]);
	return (id);
}

int		main(int ac, char **av)
{
	// int				*res[NB_PHIL];
	int				i;
	pthread_t		phil[NB_PHIL];
	// srand(time(NULL));
// creer les fourchettes
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_init(&fourchettes[i], NULL);
// creer les philosophes-threads et applique la routine a chacun
	i = -1;
	while (++i < NB_PHIL)
	{
		int	*id_start = malloc(sizeof(int));
		*id_start = i;
		if (pthread_create(&phil[i], NULL, &routine, id_start) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
	}
// arreter les philosophes-threads
	i = -1;
	while (++i < NB_PHIL)
	{
		int	*id_recup;
		if (pthread_join(phil[i], (void**) &id_recup) != 0)
		{
			perror("Failed to join thread");
			return (2);
		}
		free(id_recup);
	}
// detruire les fourchettes
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_destroy(&fourchettes[i]);
// print final result
	i = -1;
	while (++i < NB_PHIL)
	{
		// printf(""C_G_GREEN"[%i] "C_RES"Main res: %p\n", i, res[i]);
		// printf(""C_G_RED"Result: %d"C_RES"\n", *res[i]);
		// free(res[i]);
	}
	// while (1);
	return (0);
}