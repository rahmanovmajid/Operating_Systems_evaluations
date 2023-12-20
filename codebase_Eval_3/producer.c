#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>


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
	
	MEM *M = memory();					
	sem_init(&M->mutex,1,1);
	sem_init(&M->empty,1,10);
	sem_init(&M->full,1,0);

	int n;
	MEM *S = memory();
	for(int i = 0; 1 ; i++) {
		sem_wait(&S->empty); 															
		sem_wait(&S->mutex);
		sem_getvalue(&S->full,&n);
		(S->buff)[n] = i; 													
		printf("Producer: I made a computer no.[%d]\n", i);
		sem_post(&S->mutex);
		sem_post(&S->full); 												
		sleep(1);
	}
}