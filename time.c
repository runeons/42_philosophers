#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "color_shell.h"

/*
** Returns the timestamp in milliseconds
*/

long    get_time(void)
{
    struct timeval  tp;
    long            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

/*
** Prints time, sleeps 200ms, repeats!
*/

void	millisleep(long to_sleep, long last_time, long start_time)
{
	long	time_left;

	time_left = last_time + to_sleep - (get_time() - start_time);
	while (time_left > 0)
	{
		time_left = last_time + to_sleep - (get_time() - start_time);
		usleep(1 * 1000);
		time_left = last_time + to_sleep - (get_time() - start_time);
	}
}

int main(void)
{
    long start_time;
    long last_time;

		// Remember when we started
    start_time = get_time();
    while (1)
    {
				// Print time from start, in ms
        printf(""C_RED"%15ld"C_RES"\n", last_time = (get_time() - start_time));

				// Sleep 200 times 1000 microseconds (1 millisecond)
        // usleep(200 * 1000);
        millisleep(200, last_time, start_time);
    }
}