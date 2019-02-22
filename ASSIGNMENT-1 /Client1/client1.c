#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <time.h>

typedef struct messageBuffer { 
  int  msg_qid;
  long msg_type; 
  char msg_text[1024]; 
} message; 
  


int main() 
{ 
  key_t senderKey,receiverKey;
  struct timeval startTime, endTime;
  message messageReceive, messageSend;
  

  messageReceive.msg_type=1;
  messageSend.msg_type=1;

  //Generate Unique identifier keys...
  senderKey = ftok("/Users/narayani/Desktop/IIITB/Sem8/RTOS/ASSIGNMENT-1 /EchoEngine/server.c", 65); 
  receiverKey= ftok("client1",'0'); 
    
    
  int client_qid,server_qid;
    
  printf ("Enter your message: ");
  if((server_qid= msgget(senderKey, 0666 | IPC_CREAT))==-1){//Connecting to Server
      perror("server:msgget");
      exit(1);
    }
    
  
  if((client_qid=msgget(receiverKey, 0666|IPC_CREAT))==-1){     //Creating Client Queue
    perror("msgget");
    exit(1);
  }

  messageSend.msg_qid=client_qid;
//Waiting for input 
  while(fgets(messageSend.msg_text, 198, stdin)){

    gettimeofday(&startTime, NULL);                      //Note the start time and save it in startTime

    if (msgsnd(server_qid, &messageSend, sizeof(message), 0) == -1){
     perror ("client: msgsnd");
     exit (1);
    }

    if( msgrcv(client_qid, &messageReceive, sizeof(struct messageBuffer), 0, 0)==-1){
      perror("client:msgrcv");
      exit(1);
    }

  //Note the time when message is received from the server
    gettimeofday(&endTime, NULL);
    long seconds = (endTime.tv_sec - startTime.tv_sec);         
    long micros = ((seconds * 1000000) + endTime.tv_usec) - (startTime.tv_usec);
    printf("Message received from the server: %s\n", messageReceive.msg_text);
    printf("Time taken is %d seconds and %d micros\n", seconds, micros);


    printf ("Enter your message: ");
  }
   if (msgctl (client_qid, IPC_RMID, NULL) == -1){ 
      perror ("client: msgctl");
      exit (1);
    }

    return 0; 
}
