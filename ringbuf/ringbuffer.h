#import <stdio.h>
#import <pthreads.h>
#import <time.h>
#define BUFFER_SIZE 10

typedef struct {
	int value; 			// Value to be passed to consumer
	int consumer_sleep; // Time (in ms) for consumer to sleep
	int line; 			// Line number in input file 
	int print_code; 	// Output code; see below 
	int quit; 			// NZ if consumer should exit
} message_t;

struct ringBuffer{
	message_t buffer[BUFFER_SIZE];
	int nextFullIndex;
	int nextEmptyIndex;
	pthread_mutex_t mutex;
};

int count; // (global) count keeps the sum of all values recieved by the consumer

int sleep_millis(int sleep_nanos);
int consumer(void);
int producer(void);