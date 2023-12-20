#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>


void *increment(void *arg) { //a pointer function for demands of pthread_create() function
    
    struct timespec ts;
    srand(time(NULL)); // to get completely random numbers from rand() each time.If we don't bind it to time() rand() will return the same numbers 
    
    for (int i = 0 ; i < 10 ; i++ ) {   
        ts.tv_nsec = (((double)rand()) / RAND_MAX) * 1000000000;  // rand returns integer therefore casting to double to successfully divide into RAND_MAX
        // and why divide into RAND_MAX is because the largest int it can return is no more than RAND_MAX
        // so dividing into it and multiplying by 10 to the power of 9 will give the exact amount of zeros to represent nanoseconds 
        nanosleep(&ts, NULL);
        printf("Thread #%d <---> ", * (int *) arg);
        printf("slept for %ld nanoseconds and ",ts.tv_nsec);
        printf("counting:  %d\n",i);
    }

    printf("Thread #%d has finished its job.\n",* (int *) arg);
    return NULL;
}

int main (int argc, char *argv[]) {
   
    int nThread;
    printf("Enter the number of threads you want to generate: ");
    scanf("%d",&nThread);

    int th_number[nThread];
    for ( int i = 0 ; i < nThread ; i++) {  th_number[i] = i+1; }  
    pthread_t tids[nThread]; //array of pthread_t to store threads and do further operations
    
    for (int i = 0 ; i < nThread ; i++) {
        pthread_create(&tids[i], NULL, &increment,&th_number[i]);
    }   
    for (int i = 0 ; i < nThread ; i++) {
        pthread_join(tids[i], NULL);
    }
    
    return 0; 
}