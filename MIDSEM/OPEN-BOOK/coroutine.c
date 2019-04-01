#include<stdio.h> 
#include<string.h> 
#include<pthread.h> 
#include<stdlib.h> 
#include<unistd.h> 


pthread_t threadID[2]; 
int count = 0;

pthread_mutex_t threadLocks[2]; 
pthread_mutex_t mainLock;

char str[100] = "\0";



// Create a co-routine
int CreateCR(void *(*f)())
{
	pthread_create(&(threadID[count++]), NULL, f, NULL);
	pthread_mutex_lock(&mainLock);
	return count - 1;
}

// Send data to global data
void CopyString(int crid, char *msg)
{
	strcpy(str, msg);
	pthread_mutex_unlock(&(threadLocks[crid]));
	pthread_mutex_lock(&mainLock);
}


// Kill a co routine
void kill(int crid)
{
	pthread_cancel(threadID[crid]);
	pthread_mutex_destroy(&(threadLocks[crid])); 
}

// co routine 1
void* CR1() 
{ 
	int crid = count - 1;
	
	int i;

	for(i=1;i>0;i++)
	{
		pthread_mutex_unlock(&mainLock);
		pthread_mutex_lock(&(threadLocks[crid]));
		printf("Loop %d\n", i);  
	}

	return NULL; 
} 

// co routine 2
void* CR2() 
{ 
	int crid = count - 1;
	
	while(1){
		pthread_mutex_unlock(&mainLock);
		pthread_mutex_lock(&(threadLocks[crid]));		
		printf("Message %s\n", str);  
	}
	return NULL; 
} 


int main() 
{ 	
	// Initialize
	int i;
	for(i = 0; i < 2; i++)
	{
		pthread_mutex_init(&(threadLocks[i]), NULL);
		pthread_mutex_lock(&(threadLocks[i]));
	}

	pthread_mutex_init(&mainLock,NULL);
	pthread_mutex_lock(&mainLock);


	// Create some co routines 
	int cr1 = CreateCR(&CR1);
	int cr2 = CreateCR(&CR2);

	// Execute the threads
	// CopyString(cr2, "X");
	// CopyString(cr2, "Y");
	CopyString(cr1, "C");    //Message doesn't matter for loop functions
	CopyString(cr1, "S");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");
	CopyString(cr1, "Q");

	// Kill the routines
	kill(cr1);
	kill(cr2);

	return 0; 
} 