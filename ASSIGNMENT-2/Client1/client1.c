// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define PORT 8000
  
int main(int argc, char const *argv[]){
    struct timeval start, end;
    struct sockaddr_in clientAddress;
    int sock = 0;
    int readValue;
    struct sockaddr_in serverAddress;
    char readBuffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error while creating the socket \n");
        return -1;
    }
  
    memset(&serverAddress, '0', sizeof(serverAddress));
  
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("172.16.133.14");
    // char ipAddress[100];
    // printf("Please input IP address: \n");
    // scanf("%s", ipAddress);
    // printf("Hey\n");

    // char ipAddress[100] = "127.0.0.1";

    // Convert IPv4 and IPv6 addresses from text to binary form
    // if(inet_pton(AF_INET, ipAddress , &serverAddress.sin_addr)<=0){
    //     printf("\nError in Address\n");
    //     return -1;
    // }
  
    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        printf("\nError while connect\n");
        return -1;
    }
    
    // char userBuf[20];

    while(1) {

        memset(readBuffer, 0, 1024);   
        
        scanf("%s", &readBuffer);
        gettimeofday(&start, NULL);

        send(sock , readBuffer , strlen(readBuffer) , 0 );
        
        printf("Waiting for response from server.\n");

        memset(readBuffer, 0, 1024);   
        readValue = read( sock , readBuffer, 1024);

        gettimeofday(&end, NULL);

        long seconds = (end.tv_sec - end.tv_sec); 
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

        printf("Contents of the file are:  %s\n", readBuffer);

        printf("This took %ld seconds %ld microseconds\n", seconds, micros);
        
    }

    return 0;
}