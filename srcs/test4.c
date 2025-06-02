# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>

int arr[] = {1, 3, 5, 7, 9, 11, 13, 15 ,17, 19};

void		*execute_phoil(void *arg)
{
    int sum = 0;
    int i = *(int *)arg;

    for (int j = 0; j < 5; j++){
        sum += arr[i + j];
    }
    printf("sum: %d\n", sum);
    *(int *)arg = sum;
    return (arg);
}

int main()
{
    int i;

	pthread_t th[10];
    for (i = 0; i < 2; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &execute_phoil, a) != 0)
            return (1);
    }
    int go_sum = 0;
    for (i = 0; i < 2; i++)
    {
        int *r;
        if (pthread_join(th[i], (void *)&r) != 0)
        {
            printf("Falied join in index: %d\n", i);
            return (10);
        }
        go_sum += *r;
        free(r);
    }
    printf("THe glable value %d\n", go_sum);
}