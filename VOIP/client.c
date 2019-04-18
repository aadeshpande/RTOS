#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 8000


#define PORT 8888


int main(int argc, char const *argv[]){
    struct timeval start, end;
    struct sockaddr_in clientAddress;
    int sock = 0;
    int readValue;
    struct sockaddr_in serverAddress;
    char readBuffer[1024] = {0};

    //Pulse Audio object specifiactions
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 42500,
        .channels = 1
    };

    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    }


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error while creating the socket \n");
        return -1;
    }
  
    memset(&serverAddress, '0', sizeof(serverAddress));
  
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
      
    // // Convert IPv4 and IPv6 addresses from text to binary form
    // if(inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr)<=0){
    //     printf("\nError in Address\n");
    //     return -1;
    // }
  
    if (connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        printf("\nError while connect\n");
        return -1;
    }
    
    // char userBuf[20];

    while(1){

    	uint8_t buf[BUFSIZE];

    	/* Record some data ... */
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n", pa_strerror(error));
            exit(0);
        }
        
        send(sock , buf , strlen(readBuffer) , 0 );
        
        printf("Sent Data to server!\n");

        // memset(readBuffer, 0, 1024);   
        // readValue = read( sock , readBuffer, 1024);

        // gettimeofday(&end, NULL);

        // long seconds = (end.tv_sec - end.tv_sec); 
        // long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

        // printf("Contents of the file are:  %s\n", readBuffer);

        // printf("This took %ld seconds %ld microseconds\n", seconds, micros);
        
    }

    return 0;
}
