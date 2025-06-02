# include <stdio.h>
# include <pthread.h>

typedef struct s_phoil
{
	int counter;
	int target;
	pthread_t thread;

} t_phoil;

pthread_mutex_t mutex;
int inc = 0;
void		*execute_phoil(void *test)
{
	int i = 0;
	int *ph = (int *)test;
	while (i < 1000000)
	{
		pthread_mutex_lock(&mutex);// protect procces
		inc++;
		pthread_mutex_unlock(&mutex);
		i++;
	}
	(*ph)++;
	return (NULL);
}

int main()
{
	t_phoil thread;
	thread.counter = 0;
	pthread_t th[4];
	pthread_mutex_init(&mutex, NULL);
	int i = -1;

	while (++i < 4)
	{
		if (pthread_create(th + i, NULL, execute_phoil ,&thread.counter) != 0)
		{
			printf("ERROR");
			return (1);
		}
		printf("THE thread %d started execution\n", 1+i);
	}
	i = -1;
	while (++i < 4)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			printf("ERROR");
			return (1);
		}
		printf("THE thread %d finshed execution\n", 1+i);
	}
	printf("%d\n", inc);
}