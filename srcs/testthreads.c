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
void        *execute_phoil(void *test)
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
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&t1, NULL, execute_phoil ,&thread.counter);
    pthread_create(&t2, NULL, execute_phoil ,&thread.counter);
    pthread_create(&t3, NULL, execute_phoil ,&thread.counter);
    pthread_create(&t4, NULL, execute_phoil ,&thread.counter);
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_mutex_destroy(&mutex);
    // printf("%d\n", thread.counter);
    printf("%d\n", inc);
}