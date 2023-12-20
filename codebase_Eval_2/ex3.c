#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long int nGlobal = 0;
pthread_mutex_t lock;

void *increment(void *arg) {
    
    pthread_mutex_lock(&lock);      //to intervene another thread get past this line so the counter won't be incremented chaotically

    printf("Thread #%d is doing its job now...\n",  *(int *) arg);
    for ( int i = 0 ; i < 1000000 ; i++ ) { 
        nGlobal++; 
    }
    printf("Thread #%d finished incrementing and value of the counter is: %d\n\n",* (int *) arg, nGlobal);
    
    pthread_mutex_unlock(&lock);    // once the job done, we can unlock to let other threads do their work.

    return NULL;
}

int main() {

    if (pthread_mutex_init(&lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    }

    pthread_t tid[4];

    int th_number[] = { 1,2,3,4 };

    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_create(&tid[i], NULL, &increment, &th_number[i]);
    }

    for ( int i = 0 ; i < 4 ; i++ ) {
        pthread_join(tid[i], NULL);
    }

    printf("Global value at the end is : %d\n",nGlobal);

    return 0;
}