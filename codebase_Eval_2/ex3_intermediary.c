#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int nGlobal = 0;

void *increment(void *arg) {

    printf("Thread #%d is doing its job now...\n", * (int *) arg);
    for ( int i = 0 ; i < 1000000 ; i++ ) nGlobal++;
    printf("Thread #%d finished incrementing and value of the counter is: %d\n", * (int *) arg, nGlobal);
    return NULL;
}

int main() 
{
    pthread_t tid[4];
    int th_number[] = { 1,2,3,4 };

    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_create(&tid[i], NULL, increment, &th_number[i]);
    }
    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_join(tid[i], NULL);
    }
    printf("Global value at the end is : %d\n",nGlobal);

    return 0;
}