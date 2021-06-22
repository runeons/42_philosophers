#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <sys/time.h>
# include <unistd.h>
# include "color_shell.h"

extern int	g_end;
pthread_mutex_t mutex;

// none dies
// # define NB_PHIL 4
// # define TIME_TO_DIE 410
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 200

// // one dies
# define NB_PHIL 4
# define TIME_TO_DIE 310
# define TIME_TO_EAT 200
# define TIME_TO_SLEEP 100
//
// none dies
// # define NB_PHIL 5
// # define TIME_TO_DIE 800
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 200
//
// one dies
// # define NB_PHIL 5
// # define TIME_TO_DIE 310
// # define TIME_TO_EAT 200
// # define TIME_TO_SLEEP 200
// # define EATING_TIMES 4
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

long	ft_atoi_long(char const *s);

void	init_phil(t_shared *shared, t_phil *phil, int id);
void	init_shared(t_shared *shared);

int		print_error(char *msg, t_phil *phil);
void	print_rendu(t_phil *phil, int option);
int		change_state_and_print(t_phil **phil, int new_state);

int    get_time(void);
void	millisleep(int to_spend, int current_time, int starting_time);
int		ret_current_time(t_phil phil);

int		take_fork(t_phil **phil, int option);
int		take_forks(t_phil **phil, int nb_phil, int id);
int		fork_mutexes(int option, t_shared *shared);

#endif