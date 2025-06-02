# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <stdlib.h>

typedef struct s_phoil
{
	int counter;
	int target;
	pthread_t thread;

} t_phoil;

void		*execute_phoil(void *test)
{
    int value = (rand() % 6) + 1;
    int *result = malloc(sizeof(int));
    *result = value;
    printf("Result value is %p\n", result);
    return ((void*)result);
}

int main()
{
    int *res;
    srand(time(NULL));

	pthread_t th;
    if (pthread_create(&th, NULL, &execute_phoil, NULL) != 0)
        return (1);
    if (pthread_join(th, (void *)&res) != 0)
        return (1);
    printf("Result value is %p\n", res);
    printf("Result value is %d\n", *res);
}