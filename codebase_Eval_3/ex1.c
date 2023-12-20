#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>


int read_input(int *input){
    int errno;
    printf("Enter the value you want to add up\n");
    errno = scanf("%d", input);
    if(errno == -1) return -1; // Ctrl+D to stop entering
    else            return 1;
}


int compute(int *shrd_value) {
    int input;
    for(;read_input(&input) != -1;) {
        *shrd_value += input;
    }
    return 0;  
}



int main (int argc, char *argv[]) {

    int *shrd_value;
    int shm_id;

    key_t shm_key;
    shm_key = ftok ("/dev/null", 65);
    if (shm_key == -1) {
        perror("ftok error"); 
    }
    shm_id = shmget (shm_key, 4096 , IPC_CREAT | 0666);

    if (shm_id == -1) {
        perror("shmget error");
    }
    
    unsigned int sem_value1 = 1;
    unsigned int sem_value2 = 0;
    sem_t *sem1 = sem_open("semaphore1", O_CREAT | O_EXCL, 0644, sem_value1);
    sem_t *sem2 = sem_open("semaphore2", O_CREAT | O_EXCL, 0644, sem_value2);
    

    shrd_value = shmat(shm_id, NULL, 0); 

    pid_t pid;
    pid = fork(); //create child process

    //fork() error handling
    if(pid < 0) { 
        perror("fork error");
        sem_unlink("semaphore1"); 
        sem_close(sem1);  
        sem_unlink("semaphore2"); 
        sem_close(sem2); 
        exit(EXIT_FAILURE);
    }

    else if(pid == 0){
        printf("I'm child and my pid is: %d\n", pid);
        sem_wait(sem1);
        if( compute( shrd_value) == 0) {
            sem_post(sem2);
            exit(EXIT_SUCCESS);
        }
    }

    else { 
        sem_wait(sem2);
        printf("\nShared value is %d\n\n", *shrd_value);
        shmdt(shrd_value); 
        // detach the shared memory value from the @ of shrd mem
        sem_unlink("semaphore1"); 
        sem_close(sem1); 
        sem_unlink("semaphore2"); 
        sem_close(sem2);
        (shmctl(shm_id, IPC_RMID, 0) == -1) ? perror("shmctl error") : 0;  
        sem_post(sem1);
        exit(0);
    }



    return 0;
}