#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10

void *function (void *arg) 
{
    printf("%d\n",* (int *) arg);
    return NULL;
}

void error (char *msg) 
{
    perror(msg);  
    exit(1);
}


int main (int argc, char *argv[]) {

    pthread_t tid [MAX]; int i;
    for ( i = 0; i < MAX; i++ ) {
        if ( pthread_create(&tid[i], NULL, &function, (void *) &i) == -1 )
            error("pthread create");
    }

    for ( i = 0; i < MAX; i++ ) {
        if ( pthread_join(tid[i], NULL) == -1 )
            error("pthread join");
    }



    return 0;
}
