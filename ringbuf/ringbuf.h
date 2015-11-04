#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define BUFFER_SIZE 10

typedef struct {
	int value; 			// Value to be passed to consumer
	int consumer_sleep; // Time (in ms) for consumer to sleep
	int line; 			// Line number in input file 
	int print_code; 	// Output code; see below 
	int quit; 			// NZ if consumer should exit
} message_t;

typedef struct {
	message_t buffer[BUFFER_SIZE];
	int front; 			// The index of the next thing to be consumed
	int rear;  			// The index of the next place to put something that is produced
	int numElements; 	//Number of messages currently in the buffer.
	pthread_mutex_t lock;
	pthread_cond_t condVar;
} ringBuffer_t;

int count = 0; 			// (global) count keeps the sum of all values recieved by the consumer
int line_number = 0;
ringBuffer_t buf; 		// This is the global variable that represents the ring buffer type

void sleep_millis(int sleep_time);
void* consumer(void *vargp);
void* producer(void *vargp);
void error_checker(int error_code, char *str);