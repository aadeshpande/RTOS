#include<unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 

typedef struct messageBuffer { 
    int  msg_qid;
    long msg_type; 
    char msg_text[1024]; 
} message; 



int main(void)
{
  message messageReceive, messageSend;
  key_t key;
  int serverQid;

  //Generate Unique identifiers
  if((key= ftok("/Users/narayani/Desktop/IIITB/Sem8/RTOS/ASSIGNMENT-1 /EchoEngine/server.c", 65))==-1){
    perror("ftok");
    exit(1);
  }

  //Creating the server queue
  if((serverQid = msgget(key, 0666 | IPC_CREAT))==-1){
    perror("msgget");
    exit(1);
  }

  while(1){
  msgrcv(serverQid, &messageReceive, sizeof(message), 0, 0); 

  
  //Change capitalisation
  if(messageReceive.msg_text[0]<='z' && messageReceive.msg_text[0]>='a')
        messageReceive.msg_text[0]= (char)(messageReceive.msg_text[0]-32);


    messageSend=messageReceive;

    int client_id=messageReceive.msg_qid;

    //Send reply to Client connected
    if(msgsnd(client_id, &messageSend, sizeof(message),0)==-1){
      perror("msgsnd");
      exit(1);
    }
  }
}