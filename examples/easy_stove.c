#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// chefs = threads
// stove = shared data (+mutex)

pthread_mutex_t stoveMutex;
int stoveFuel = 100;

void	*routine(void *args)
{
	pthread_mutex_lock(&stoveMutex);
	int fuelNeeded = (rand() % 30);
	if (stoveFuel - fuelNeeded < 0)
		printf("No more fuel... going home\n");
	else
	{
		stoveFuel -= fuelNeeded;
		usleep(500000);
		printf("Fuel left %d\n", stoveFuel);
	}
	pthread_mutex_unlock(&stoveMutex);
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	pthread_t th[10];
	pthread_mutex_init(&stoveMutex, NULL);
	for (int i = 0; i < 10; i++) {
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
			perror("Failed to create thread");
		}
	}

	for (int i = 0; i < 10; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join thread");
		}
	}
	pthread_mutex_destroy(&stoveMutex);
	return 0;
}