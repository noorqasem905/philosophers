# include <stdio.h>
# include <pthread.h>
# include <time.h>
# include <stdlib.h>

int arr[] = {1, 3, 5, 7, 9, 11, 13, 15 ,17, 19};

void		*execute_phoil(void *arg)
{
    int i = *(int *)arg;
    printf("the array result: %d\n", arr[i]);
    free(arg);
}

int main()
{
    int i;

	pthread_t th[10];
    for (i = 0; i < 10; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &execute_phoil, a) != 0)
            return (1);
    }
    for (i = 0; i < 10; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("Falied join in index: %d\n", i);
            return (10);
        }
    }
}
//r using threads #1 