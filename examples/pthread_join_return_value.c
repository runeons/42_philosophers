#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

void* roll_dice() {
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;
    // printf("%d\n", value);
    printf("Thread result: %p\n", result);
    return (void*) result;
}

int main(int argc, char* argv[]) {
    int			*res[4];
	int			i;
    pthread_t	th[4];

	srand(time(NULL));
	for (i = 0; i < 4; i++)
	{
		if (pthread_create(&th[i], NULL, &roll_dice, NULL) != 0) {
			return 1;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], (void**) &res[i]) != 0) {
			return 2;
		}
	}
	for (i = 0; i < 4; i++)
	{
		printf("Main res: %p\n", res[i]);
		printf("Result: %d\n", *res[i]);
		free(res[i]);
	}
    return 0;
}