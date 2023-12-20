#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define PRODUCER_SLEEP_SEC 1
#define KEY 1010

typedef struct memory{
	int buff[10];
	sem_t mutex, empty, full;
}MEM;


MEM *memory() {
	key_t key = 1010;
	int shmid;
	shmid = shmget(key, sizeof(MEM), IPC_CREAT | 0666);
	return (MEM *) shmat(shmid, NULL, 0);
}


int main(int argc, char *argv[]) {

	int n;
	MEM *S = memory();

	for(;1;) {
		sem_wait(&S->full); 			
		sem_wait(&S->mutex); 													
		sem_getvalue(&S->full,&n); 															
		printf("Consumer: I bought the computer no.[%d]\n", (S->buff)[n]);
		sem_post(&S->mutex); 															
		sem_post(&S->empty); 															
		sleep(3);
	}


	return 0;
}