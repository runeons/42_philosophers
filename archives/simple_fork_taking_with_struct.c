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
** gcc simple_fork_taking_with_struct.c && ~/valgrind_fork/bin/valgrind ./a.out
*/

pthread_mutex_t	fourchettes[NB_FORK];
int				availableForks = 2;

typedef struct		s_args
{
	int	id;
	// int	eating_times[NB_PHIL];
}					t_args;

void	*routine(void *args)
{
    int	local_id;
	int	i;

	local_id = ((t_args *)args)->id;
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
	return (args);
}

int		main(int ac, char **av)
{
	// int				*res[NB_PHIL];
	int				i;
	pthread_t		phil[NB_PHIL];
	t_args			*args;
	// srand(time(NULL));

// creer les fourchettes
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_init(&fourchettes[i], NULL);
// creer les philosophes-threads et applique la routine a chacun
// malloc dans la loop car malloc pour chaque thread
	i = -1;
	while (++i < NB_PHIL)
	{
		if (!(args = malloc(sizeof(t_args))))
			return (-1);
		args->id = i;
		// int	*id_start = malloc(sizeof(int));
		// *id_start = i;
		if (pthread_create(&phil[i], NULL, &routine, args) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
	}
// arreter les philosophes-threads
// free dans la loop car free pour chaque thread
	i = -1;
	while (++i < NB_PHIL)
	{
		if (pthread_join(phil[i], (void**) &args) != 0)
		{
			perror("Failed to join thread");
			return (2);
		}
		free(args);
	}
// detruire les fourchettes
	i = -1;
	while (++i < NB_FORK)
		pthread_mutex_destroy(&fourchettes[i]);
// while (1);
	return (0);
}