#include "ringbuf.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_NANO_TIME 999999999
#define NANO_TO_MILLIS_FACTOR 1000000
/*
	consumer function should read from the buffer and change the 
	buffer's head accordingly.
*/
void* consumer(void *vargp) {
	//Create a flag which lets us know if we have hit the end of file
	int at_EOF = 0;

	//Go through the loop until we have hit this flag
	while(!at_EOF) {

		// Grab the lock so we can check the number of elements in the buffer 
		int lock_code = pthread_mutex_lock(&buf.lock);
		error_checker(lock_code, "lock_code consumer");

		if (buf.numElements == 0) {
			int wait_code = pthread_cond_wait(&buf.condVar, &buf.lock);
			error_checker(wait_code, "wait_code consumer");
		}

		// give up the lock before waiting so there are no deadlocks
		int unlock_code = pthread_mutex_unlock(&buf.lock);
		error_checker(unlock_code, "unlock_code consumer");

		message_t newMessage = buf.buffer[buf.front];
		buf.front = (buf.front + 1) % BUFFER_SIZE;

		// if quit is non-zero, then return 1
		if (newMessage.quit) {
			printf("Final sum is %d\n", count);
			at_EOF = 1;
			break;
		}

		// consume the message
		sleep_millis(newMessage.consumer_sleep);

		// add the value to the count accumulator
		count += newMessage.value;

		// print if appropriate print code
		if (newMessage.print_code & 2) {
			printf("Consumed %d from input line %d; sum = %d\n", newMessage.value, 
					newMessage.line, count);
		}

		//we want to look at num elements so get the lock
		int lock_code2 = pthread_mutex_lock(&buf.lock);
		error_checker(lock_code2, "lock_code2 consumer");

		//if the buffer was full then someone could have been waiting so lets send a signal
		if (buf.numElements == BUFFER_SIZE){
			int signal_code = pthread_cond_signal(&buf.condVar);
			error_checker(signal_code, "signal_code consumer");
		}
		// after consuming, decrement the number of elements counter
		--buf.numElements;

		int unlock_code2 = pthread_mutex_unlock(&buf.lock);
		error_checker(unlock_code2, "unlock_code2 consumer");
	}
	pthread_exit(NULL);
}

/* 
	producer function should add to the buffer and change the 
	buffer's rear accordingly.
*/
void* producer(void *vargp) {
	
	//Create a flag which lets us know if we have hit the end of file
	int at_EOF = 0;

	//Go through the loop until we have hit this flag
	while(!at_EOF){

		//Grab the lock so we can check the number of elements in the buffer 
		int lock_code = pthread_mutex_lock(&buf.lock);
		error_checker(lock_code, "lock_code producer");

		//set the flag
		if (buf.numElements == BUFFER_SIZE) {
			int wait_code = pthread_cond_wait(&buf.condVar, &buf.lock);
			error_checker(wait_code, "wait_code producer");
		}

		// give up the lock before waiting so there are no deadlocks
		int unlock_code = pthread_mutex_unlock(&buf.lock);
		error_checker(unlock_code, "unlock_code producer");

		message_t newMessage;
		int producer_sleep = 0;

		//Read in the next message
		int scan_result = scanf("%d %d %d %d", &newMessage.value, &producer_sleep,
			&newMessage.consumer_sleep, &newMessage.print_code);

		if (scan_result == EOF) { //if we are at end of file exit
			newMessage.quit = 1;
			at_EOF = 1;
		}

		// otherwise add things to the buffer
		else { 
			newMessage.quit = 0;
			newMessage.line = ++line_number; 
			sleep_millis(producer_sleep);

			//See if we need to print a status update
			if (newMessage.print_code & 1) {
				printf("Produced %d from input line %d\n", newMessage.value,
					newMessage.line);
			} 
		}

		// Add the message to the buffer
		buf.buffer[buf.rear] = newMessage;

		// Set rear to the next valid index in the ringbuffer
		buf.rear = (buf.rear + 1) % BUFFER_SIZE;

		//we want to look at num elements so get the lock
		int lock_code2 = pthread_mutex_lock(&buf.lock);
		error_checker(lock_code2, "lock_code2 producer");

		//if the buffer was empty then someone could have been waiting so lets send a signal
		if (buf.numElements == 0){
			int signal_code = pthread_cond_signal(&buf.condVar);
			error_checker(signal_code, "signal_code producer");
		}
		// after producing, increment the number of elements counter
		++buf.numElements;

		int unlock_code2 = pthread_mutex_unlock(&buf.lock);
		error_checker(unlock_code2, "unlock_code2 producer");

	}
	pthread_exit(NULL);
}

/* 
	Given time in ms, sleep for time using sleep in ns.
*/
void sleep_millis(int sleep_time) {
	
	struct timespec tim;

	if (sleep_time >= 1000) { 
		sleep_millis(sleep_time - (MAX_NANO_TIME / NANO_TO_MILLIS_FACTOR));
		tim.tv_sec = 0;
		tim.tv_nsec = MAX_NANO_TIME;
		nanosleep(&tim, NULL);
	}

	else if (sleep_time == 0)
		return;

	else {
		tim.tv_sec = 0;
		tim.tv_nsec = sleep_time * NANO_TO_MILLIS_FACTOR;
		nanosleep(&tim, NULL);
	}

	return;
}

void error_checker(int error_code, char *str) {
	if (error_code != 0) {
		printf("Error with code: %d You should probably use GDB \n", error_code);
		printf("%s\n", str);
		exit(error_code);
	}
	return;
}

int main(void) {
	setlinebuf(stdout);

	// initialize the mutex
	int initCode = pthread_mutex_init(&buf.lock, NULL);
	error_checker(initCode, "init mutex main");

	// initialize the condition variable condVar
	int condErrorCode = pthread_cond_init(&buf.condVar, NULL);
	error_checker(condErrorCode, "condVar main");

	// Declare threads for the producer and consumer
	pthread_t producerThread, consumerThread;

	// create the thread for the consumer
	int create1 = pthread_create(&consumerThread, NULL, consumer, NULL);
	error_checker(create1, "create1 main");

	// create the thread for the producer
	int create2 = pthread_create(&producerThread, NULL, producer, NULL);
	error_checker(create2, "create2 main");

	// join consumer thread
	int join1 = pthread_join(consumerThread, NULL);	
	error_checker(join1, "join1 main");

	// join producer thread
	int join2 = pthread_join(producerThread, NULL);
	error_checker(join2, "join2 main");

	return 1;
}