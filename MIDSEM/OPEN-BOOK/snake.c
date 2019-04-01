#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<time.h>

pthread_t threadID[4]; 
int count = 0;

pthread_mutex_t threadLocks[4]; 
pthread_mutex_t mainLock;

char str[100] = "\0";
int winner = 0;



// CreateCR a co-routine
int CreateCR(void *(*f)()){
	pthread_create(&(threadID[count++]), NULL, f, NULL);
	pthread_mutex_lock(&mainLock);
	return count - 1;
}

// Send data to global data
void controlFlow(int crId, char * msg){
	strcpy(str, msg);
	pthread_mutex_unlock(&(threadLocks[crId]));
	pthread_mutex_lock(&mainLock);
}


// Kill a co routine
void kill(int crId){
	pthread_cancel(threadID[crId]);
	pthread_mutex_destroy(&(threadLocks[crId])); 
}



void* GamePlayers(){
	srand(time(0)); 
	int crId = count - 1;
	int level = 0;
	int dice;
	while(1)
	{
		pthread_mutex_unlock(&mainLock);
		pthread_mutex_lock(&(threadLocks[crId]));	
		dice = rand() % 6 + 1;

		level += dice;
		if(level >= 100)
		{
			winner = crId + 1;
			printf("Winner: Player %d\n", crId+1);

		}
		// printf("%d %d\n", crId+1, level);

	}

}


int main() { 	
	// Initialize
	int i = 0; 
	for(i = 0; i < 4; i++){
		pthread_mutex_init(&(threadLocks[i]), NULL);
		pthread_mutex_lock(&(threadLocks[i]));
	}

	pthread_mutex_lock(&mainLock);

	struct timeval start, end;


	// CreateCR some co routines 
	int c_routine1 = CreateCR(&GamePlayers);
	int c_routine2 = CreateCR(&GamePlayers);
	int c_routine3 = CreateCR(&GamePlayers);
	int c_routine4 = CreateCR(&GamePlayers);

	gettimeofday(&start, NULL);


	while(1){
		if(winner > 0)
			break;
		controlFlow(c_routine1, "A");
		if(winner > 0)
			break;
		controlFlow(c_routine2, "B");
		if(winner > 0)
			break;
		controlFlow(c_routine3, "C");
		if(winner > 0)
			break;
		controlFlow(c_routine4, "D");

	}

    gettimeofday(&end, NULL);

	long seconds = (end.tv_sec - start.tv_sec); 
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);

    printf("%ld seconds %ld microseconds\n", seconds, micros);

	// Kill the routines
	kill(c_routine1);
	kill(c_routine2);
	kill(c_routine3);
	kill(c_routine4);

	return 0; 
} 