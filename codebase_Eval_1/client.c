#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MAX 1024

typedef struct message{   
    long msg_type;
    char msg_text[MAX];
}Message;


int getQueue(void) {
    key_t k;
    int queue_id;

    k = ftok("/etc/passwd", 'A');
    if (k == -1){ //ftok verification
        perror("Something went wrong with ftok");  
        exit(0);      
    }
    queue_id = msgget(k, 0);
    if (queue_id == -1) { //msgget verification
        perror("Something went wrong with msgget");
        printf("NOTE: Check if server.c is running.\n\n");
        exit(0);
    }
    return queue_id;

}


int read_line(char text[], long* priority, int n){

    char *pText = (char *)malloc(sizeof(char));
    int length;
    
    printf("Your Message: ");
    pText = fgets(text, n, stdin);
    
    if(pText == NULL) {
        return EOF;
    }

    else {
        printf("and its Priority: ");
        scanf("%ld",priority);
        fgetc(stdin); // as scanf uses buffer, there left a \n in buffer, so we took that character, so that in next iteration no problem occured
        length = strlen(text);
        
        if(length > 0 && text[length-1] == '\n') {
            text[length-1]='\0';
        }

        return !EOF;
    }
} //end read_line()



void writer(int msg_id) {
    Message msg;

    int length;
  
    while(read_line(msg.msg_text, &msg.msg_type, MAX) != EOF) {
       
        length = strlen(msg.msg_text);
        if(msgsnd(msg_id, &msg, length+1, 0) == -1) { // msgsnd and its verification. +1 as '\0' will be sent as well. 
            perror("Something went wrong with msgsnd");
            exit(1);           
        }
       
    }

    if(msgsnd(msg_id, &msg, 0, 0) == -1) {
        perror("Something went wrong with msgsnd");
        exit(1);
    }

} //end writer()


int main () {

    // int queue_id = getQueue();
    writer(getQueue());

    return 0;
}