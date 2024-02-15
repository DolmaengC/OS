/*
	Implement the producer-consumer problem using mutex and semaphore.

	main()
		creates a buffer, a mutex, and two semaphores 
		prepare parameters for the producer and consumer threads
		launch two threads, one for the producer and the other for consumer
		wait for the specified duration
		terminate the child threads by setting a 'continue flag' to FALSE.

	A producer thread creates a text message and inserts it into the buffer.
		whenever it inserts a message into the buffer, display a message
		if the 'continue flag' becomes FALSE, terminate 

	A consumer thread deletes a text messagefrom the buffer and consumes it
		whenever it delets a message into the buffer, display a message
		if the 'continue flag' becomes FALSE, terminate 

	Try to make the output as similar to the example as possible.


	Example) ./hw6_2 1 2 10				// the producer is faster than the consumer
		producer_delay = 1, consumer_delay = 2, duration = 10
		[producer] created a message [Boys, be ambitious for Christ!].
		[producer] inserted message into the buffer (count = 1) [Boys, be ambitious for Christ!].
			[consumer] deleted message from the buffer (count = 0) [Boys, be ambitious for Christ!].
			[consumer] consuming message [Boys, be ambitious for Christ!].
		[producer] created a message [Blessed are those who are persecuted because of righteousness, for theirs is the kingdom of heaven.].
		[producer] inserted message into the buffer (count = 1) [Blessed are those who are persecuted because of righteousness, for theirs is the kingdom of heaven.].
			[consumer] deleted message from the buffer (count = 0) [Blessed are those who are persecuted because of righteousness, for theirs is the kingdom of heaven.].
			[consumer] consuming message [Blessed are those who are persecuted because of righteousness, for theirs is the kingdom of heaven.].
		[producer] created a message [Boys, be ambitious for Christ!].
		[producer] inserted message into the buffer (count = 1) [Boys, be ambitious for Christ!].
		[producer] created a message [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		[producer] inserted message into the buffer (count = 2) [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
			[consumer] deleted message from the buffer (count = 1) [Boys, be ambitious for Christ!].
			[consumer] consuming message [Boys, be ambitious for Christ!].
		[producer] created a message [Blessed are the peacemakers, for they will be called sons of God.].
		[producer] inserted message into the buffer (count = 2) [Blessed are the peacemakers, for they will be called sons of God.].
		[producer] created a message [Blessed are the peacemakers, for they will be called sons of God.].
		[producer] inserted message into the buffer (count = 3) [Blessed are the peacemakers, for they will be called sons of God.].
			[consumer] deleted message from the buffer (count = 2) [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
			[consumer] consuming message [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		[producer] created a message [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		[producer] inserted message into the buffer (count = 3) [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		[producer] created a message [Blessed are those who hunger and thirst for righteousness, for they will be filled.].
			[consumer] deleted message from the buffer (count = 2) [Blessed are the peacemakers, for they will be called sons of God.].
			[consumer] consuming message [Blessed are the peacemakers, for they will be called sons of God.].
		[producer] inserted message into the buffer (count = 3) [Blessed are those who hunger and thirst for righteousness, for they will be filled.].
		[producer] created a message [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		[main] Deleting a string to signal the producer.
		[producer] inserted message into the buffer (count = 3) [Blessed are the poor in spirit, for theirs is the kingdom of heaven.].
		Destroying mutex and semaphores
		Bye!


	Exmple2) hw6_2 2 1 10			// the consumer is faster than the producer
		producer_delay = 2, consumer_delay = 1, duration = 10
		[producer] created a message [Boys, be ambitious for Christ!].
		[producer] inserted message into the buffer (count = 1) [Boys, be ambitious for Christ!].
			[consumer] deleted message from the buffer (count = 0) [Boys, be ambitious for Christ!].
			[consumer] consuming message [Boys, be ambitious for Christ!].
		[producer] created a message [Boys, be ambitious for Christ!].
		[producer] inserted message into the buffer (count = 1) [Boys, be ambitious for Christ!].
			[consumer] deleted message from the buffer (count = 0) [Boys, be ambitious for Christ!].
			[consumer] consuming message [Boys, be ambitious for Christ!].
		[producer] created a message [Blessed are the meek, for they will inherit the earth.].
		[producer] inserted message into the buffer (count = 1) [Blessed are the meek, for they will inherit the earth.].
			[consumer] deleted message from the buffer (count = 0) [Blessed are the meek, for they will inherit the earth.].
			[consumer] consuming message [Blessed are the meek, for they will inherit the earth.].
		[producer] created a message [Blessed are the meek, for they will inherit the earth.].
		[producer] inserted message into the buffer (count = 1) [Blessed are the meek, for they will inherit the earth.].
			[consumer] deleted message from the buffer (count = 0) [Blessed are the meek, for they will inherit the earth.].
			[consumer] consuming message [Blessed are the meek, for they will inherit the earth.].
		[producer] created a message [Never give up!].
		[producer] inserted message into the buffer (count = 1) [Never give up!].
			[consumer] deleted message from the buffer (count = 0) [Never give up!].
			[consumer] consuming message [Never give up!].
		[main] Inserting a dummy string to signal the consumer.
			[consumer] deleted message from the buffer (count = 0) [Dummy].
			[consumer] consuming message [Dummy].
		Destroying mutex and semaphores
		Bye!

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0


// constants, data type, and functions for string buffer

#define MAX_LEN 256
#define BUFFER_SIZE 3

typedef struct {
	char string[BUFFER_SIZE][MAX_LEN];
	int in, out;
	int count;
} StringBuffer;

void StringBuffer_Init(StringBuffer *sb);
void StringBuffer_Insert(StringBuffer *sb, char *str);
void StringBuffer_Delete(StringBuffer *sb, char *str);



// data type and functions to solve the producer-consumer problem

typedef struct {
	StringBuffer *p_sb;
	int *p_cont;
	int delay;

	// pointers to the mutex and semaphores
	pthread_mutex_t *p_mutex;
	sem_t *p_empty, *p_full;
} ThreadParam;

void *Producer(void *arg);
void *Consumer(void *arg);


int main(int argc, char *argv[])
{
	if(argc < 4){
		printf("Usage: %s <producer_delay> <consumer_delay> <duration>\n", argv[0]);
		return 0;
	}

	int producer_delay = atoi(argv[1]);
	int consumer_delay = atoi(argv[2]);
	int duration = atoi(argv[3]);

	srand(time(NULL));

	int thread_cont = TRUE;

	printf("producer_delay = %d, consumer_delay = %d, duration = %d\n", producer_delay, consumer_delay, duration);

	pthread_mutex_t mutex;
	sem_t empty, full;

	// TO DO: initialize mutex and semaphores 
	if ( pthread_mutex_init(&mutex, NULL) != 0 ) {
		perror("Failed pthread_mutex_init");
		exit(1);
	}
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);



	StringBuffer sb;
	StringBuffer_Init(&sb);

	ThreadParam param[2];
	// TO DO: fill out param[0] for the producer
	// 	  and fill out param[1] for the consumer
	param[0].p_sb = &sb;
	param[1].p_sb = &sb;
	param[0].p_cont = &thread_cont;
	param[1].p_cont = &thread_cont;
	param[0].delay = producer_delay;
	param[1].delay = consumer_delay;
	
	// pointers to the mutex and semaphores
	pthread_mutex_t *p_mutex = &mutex;
	sem_t *p_empty = &empty;
	sem_t *p_full = &full;
	param[0].p_mutex = p_mutex;
	param[1].p_mutex = p_mutex;
	param[0].p_empty = p_empty;
	param[1].p_empty = p_empty;
	param[0].p_full = p_full;
	param[1].p_full = p_full;

	// TO DO: create two threads: one of producer, the other for consumer
	pthread_t thread[2];
	if ( pthread_create(&thread[0], NULL, Producer, &param[0]) == -1 ||
			pthread_create(&thread[1], NULL, Consumer, &param[1]) == -1 ) {
		perror("Failed pthread_create ");
		exit(1);
	}
	


	sleep(duration);		 // wait for 30 sec

	// TO DO: terminate threads by setting thread_cont to FALSE;
	thread_cont = FALSE;	




	sleep(1);

	// TO DO: Find out the reason why the following code is necessary.
	char dummy[MAX_LEN] = "Dummy";
	if(sb.count == 0){
		printf("[main] Inserting a dummy string to signal the consumer.\n");
		StringBuffer_Insert(&sb, dummy);
		sem_post(&full);
	} else {
		printf("[main] Deleting a string to signal the producer.\n");
		StringBuffer_Delete(&sb, dummy);
		sem_post(&empty);
	}

	// TO DO: wait for the producer and consumer to terminate
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	



	printf("Destroying mutex and semaphores\n");
	// TO DO: destroy mutext and semaphores
	pthread_mutex_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);


	printf("Bye!\n");			// this message MUST be displayed

	return 0;
}


// functions for StringBuffer

void StringBuffer_Init(StringBuffer *sb)
{
	sb->count = 0;
	sb->in = sb->out = 0;
}

void StringBuffer_Insert(StringBuffer *sb, char *str)
{
	if(sb->count < BUFFER_SIZE){
		strcpy(sb->string[sb->in], str);
		sb->in = (sb->in + 1) % BUFFER_SIZE;
		sb->count++;
	}
}

void StringBuffer_Delete(StringBuffer *sb, char *str)
{
	if(sb->count > 0){
		strcpy(str, sb->string[sb->out]);
		sb->out = (sb->out + 1) % BUFFER_SIZE;
		sb->count--;
	} else {
		str[0] = 0;
	}
}

// Thread function for the producer and consumer
//

// TO DO: complete Producer() and Consuminger by implementing the producer-consumer solution in the lecture slide.
//		for synchronization, use the mutex and semaphores passed through the pointers in 'param'
void* Producer(void *arg)
{
	ThreadParam *param = (ThreadParam*) arg;

	int no_message = 10;
	char message_array[][MAX_LEN] = {
		"Blessed are the poor in spirit, for theirs is the kingdom of heaven.",
		"Blessed are those who mourn, for they will be comforted.",
		"Blessed are the meek, for they will inherit the earth.",
		"Blessed are those who hunger and thirst for righteousness, for they will be filled.",
		"Blessed are the merciful, for they will be shown mercy.",
		"Blessed are the pure in heart, for they will see God.",
		"Blessed are the peacemakers, for they will be called sons of God.",
		"Blessed are those who are persecuted because of righteousness, for theirs is the kingdom of heaven.",
		"Never give up!",
		"Boys, be ambitious for Christ!"
	};

	while(*param->p_cont){
		char mesg[MAX_LEN] = "";

		// remainder section
		// TO DO: produce a message in mesg
		// 		randomly select a message from;
		strcpy(mesg, message_array[rand()%10]);

		printf("[producer] created a message [%s].\n", mesg);
		
		// entry section fill out param[0] for the producer		
		// TO DO: implement the entry section of the producer-consumer solution
		sem_wait(param->p_empty);
		pthread_mutex_lock(param->p_mutex);

		// critical section
		// TO DO: insert mesg into the string buffer
		StringBuffer_Insert(param->p_sb, mesg);	
		printf("[producer] inserted message into the buffer (count = %d) [%s].\n", param->p_sb->count, mesg);

		// exit section
		// TO DO: implement the exit section of the producer-consumer solution
		pthread_mutex_unlock(param->p_mutex);
		sem_post(param->p_full);


		// remainder section
		sleep(param->delay);
	}

	return NULL;
}

void* Consumer(void *arg)
{
	ThreadParam *param = (ThreadParam*) arg;
	while(*param->p_cont){
		char mesg[MAX_LEN] = "";

		// entry section
		// TO DO: implement the entry section of the producer-consumer solution
		sem_wait(param->p_full);
		pthread_mutex_lock(param->p_mutex);

		// critical section
		// TO DO: delete a message from the buffer and store it into mesg
		StringBuffer_Delete(param->p_sb, mesg);	
		printf("\t[consumer] deleted message from the buffer (count = %d) [%s].\n", param->p_sb->count, mesg);

		// exit section
		// TO DO: implement the exit section of the producer-consumer solution
		pthread_mutex_unlock(param->p_mutex);
		sem_post(param->p_empty);


		// remainder section
		printf("\t[consumer] consuming message [%s].\n", mesg);

		sleep(param->delay);
	}

	return NULL;
}
