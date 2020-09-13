//get prime numbers from 1-100 using threads
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int sieve[101];
int values[101];
int main()
{
    extern void *thread_function(void *arg);
    pthread_t tid[101];
    for (int i = 2; i < 101; i++) {
        sieve[i] = 1;
        values[i] = i;
    }
    for (int j = 2; j < 101; j++) {
        if( pthread_create(&tid[j], NULL, thread_function, &values[j]) != 0) {
            perror("pthread_create");
            return(1);
        }
    }
    for (int k = 2; k < 101; k++) {
        if ( pthread_join(tid[k], NULL) != 0) {
            perror("pthread_join");
            return(1);
        }
    }
    for (int h = 2; h < 101; h++) {
        if (sieve[h] == 1)
            printf("%d\n", h);
    }
    return(0);
}
void *thread_function(void *arg)
{
    int incr = *(int *)arg;
    int j = incr * 2;
    while (j <= 100) {
        sieve[j] = 0;
        j += incr;
    }
    return NULL;
}
