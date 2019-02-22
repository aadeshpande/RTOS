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
  key_t key;
  int serverQid;
  struct messageBuffer messageReceive, messageSend;
  char data[1024];

  if((key= ftok("/Users/narayani/Desktop/IIITB/Sem8/RTOS/ASSIGNMENT-1 /EchoEngine/server.c", 65))==-1){
    perror("ftok");
    exit(1);
  }

  //Creating Server Queue
  if((serverQid = msgget(key, 0666 | IPC_CREAT))==-1){
    perror("msgget");
    exit(1);
  }

  printf("Server Started with Server ID %d\n", serverQid);
  
  while(1){
    msgrcv(serverQid, &messageReceive, sizeof(struct messageBuffer), 0, 0); 

    //Get the file name
    char *p_str = strtok(messageReceive.msg_text," "); 
    char fileName[1024];

    while(p_str != NULL){
      strcpy(fileName,p_str);
      p_str = strtok (NULL, " ");
    }
    size_t fileLen = strlen(fileName) - 1;


  if (*fileName && fileName[fileLen] == '\n')  
      fileName[fileLen] = '\0';

    FILE *file_ptr;
    file_ptr = fopen(fileName,"r");

    if (file_ptr == NULL){ 
        printf( "%s file gave error while opening\n",fileName ) ; 
        perror("Couldn't open file");
    } 

    else{
        printf("Opened\n") ; 
        fgets( data, 50, file_ptr ); 
    } 
    fclose(file_ptr);

    messageSend=messageReceive; 

    strcpy(messageSend.msg_text,data); 
   
    int client_id=messageReceive.msg_qid;
   
    if(msgsnd(client_id, &messageSend, sizeof(struct messageBuffer),0)==-1){
      perror("msgsnd");
      exit(1);
    }
  }
}