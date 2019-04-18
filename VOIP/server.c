#include <errno.h> 
#include <arpa/inet.h>    
#include <netinet/in.h> 
#include <sys/time.h>  
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>    
#include <fcntl.h> 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>

#define BUFSIZE 8000

#define TRUE   1 
#define FALSE 0 
#define PORT 8888


int main(int argc, char const *argv[])
{
	int opt = TRUE;  
    int socketMaster , addrlen , socketNew , socketClient[30] , 
          i , readValue , sd;  
    int sdMax;  
    struct sockaddr_in serverAddress;  


    //Pulse Audio object specifiactions
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 42500,
        .channels = 2
    };

    pa_simple *s = NULL;
    int ret = 1;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, argv[0], PA_STREAM_PLAYBACK, NULL, "playback", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(error));
    } 
        
	char readBuffer[1025]; //data buffer of 1K 

	if( (socketMaster = socket(AF_INET , SOCK_STREAM , 0)) == 0){  
        perror("Error creating a socket");  
        exit(EXIT_FAILURE);  
	}

	if( setsockopt(socketMaster, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
    
    //type of socket created 
    serverAddress.sin_family = AF_INET;  
    serverAddress.sin_addr.s_addr = INADDR_ANY;  
	serverAddress.sin_port = htons( PORT );  

	if (bind(socketMaster, (struct sockaddr *)&serverAddress, sizeof(serverAddress))<0){  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
	printf("Listener on port %d \n", PORT);

	if (listen(socketMaster, 3) < 0){  
        perror("listen");  
        exit(EXIT_FAILURE);  
	}

	 //accept the incoming connection 
    addrlen = sizeof(serverAddress);  
	puts("Waiting for connections ...");
	

	if ((socketNew = accept(socketMaster, (struct sockaddr *)&serverAddress, (socklen_t*)&addrlen))<0){  
                perror("accept");  
                exit(EXIT_FAILURE);  
	}
	else{
		while(1){
			uint8_t buffer[BUFSIZE];
			readValue = read(socketNew,buffer,sizeof(buffer));
			printf("Received Data\n");

			if (pa_simple_write(s, buffer, (size_t) readValue, &error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(error));
            exit(0);
           	}
        }
	} 
	return 0;
}

