#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX 1024

typedef struct message {   
    long msg_type;
    char msg_text[MAX];
}Message;



void handle(Message messages[]) {

    printf("..\n...\n....\nMessages received by server with respect to their priority (nPr):\n");
    for (int nPr = 1; nPr <= 3; nPr++) {
        for(int i = 0; i < 100; i++) {
            if(nPr == messages[i].msg_type) {
                printf("Message : %s\n", messages[i].msg_text);
                printf("its Priority: %ld\n", messages[i].msg_type);  
            }
        }
    }

} //end handle()


int new_queue(void ){ //create new queue and return its id
    
    key_t k;
    int queue_id;

    k = ftok("/etc/passwd", 'A') ;
    if (k == -1){ 
        perror("Something went wrong with ftok"); 
    }
    queue_id = msgget(k, IPC_CREAT | 0666);

    if (queue_id == -1){
        perror("Something went wrong with queue_id");
    }
    
    return queue_id;
}


void dequeue(int id) {

    int err;
    err = msgctl(id, IPC_RMID, NULL);
    if (err == -1){
        perror("Something went wrong with msgctl");
    } 

} //end dequeue()


Message* reader(int queue_id, Message messages[]) {

    int length, bytes = 0;
    Message msg;
    int count = 0;

    while( (length = msgrcv(queue_id, &msg, sizeof(msg), 0, 0)) > 0) {

        strncpy(messages[count].msg_text, msg.msg_text, length);
        messages[count].msg_type = msg.msg_type;
        count++;

        bytes += length-1;
    }

    if(length == 0) {  // if the sent message is zero bytes, no need for it to be in the queue so dequeue
        dequeue(queue_id);
    }

    else if(length == -1) { //msgrcv verification
        perror("Something went wrong with msgrcv");
        exit(1);
    }

    printf("Received number of  bytes is %d\n", bytes);
    return messages;

} //end reader()


int main () {

    int queue_id = new_queue();

    Message messages[100];
    handle(reader(queue_id, messages));

    return 0;
}