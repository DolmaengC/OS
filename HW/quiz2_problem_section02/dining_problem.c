/*

	This program is the deadlock-free solution to the dining philosophers problem using monitor.
	While running, adjacent philosophers cannot eat at the same time!

	Complete the program following the instructions.
		- Use pthread_mutex_t to ensure mutual exclusion of the procedures.
		- Use pthread_cond_t for the conditional variables.

	Compilation command: gcc dining.c -o dining -pthread
	Execution: ./dining <duration>

	When submitting, rename the source file to dining_<student_id>.c

	An example is provided in a separate file 'dp_example.txt'
	Try to make the output as similar to the example as possible.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <unistd.h>

#include <pthread.h>

#define TRUE 1
#define FALSE 0

#define _DEBUG		//  enable this macro to display debug messages

// a thread-safe version of printf. The 1st parameter specifies indentation level to identify the thread.
void ExclusivePrintf(int indent, const char *format, ...);


// The deadlock-free solution to the dining philosophers problem

enum { THINKING, HUNGRY, EATING };

typedef struct {
	int state[5];				// the state of the philosophers
	pthread_mutex_t mutex;		// mutex to ensure mutual exclusion of the procedures
	pthread_cond_t self[5];		// for the conditional variables
} DiningPhil;

void DiningPhil_Init(DiningPhil *dp);
void DiningPhil_Destroy(DiningPhil *dp);
void DiningPhil_PickUp(DiningPhil *dp, int id);
void DiningPhil_PutDown(DiningPhil *dp, int id);
void DiningPhil_Test(DiningPhil *dp, int id, int caller);
void DiningPhil_Check(DiningPhil *dp);


void DiningPhil_Init(DiningPhil *dp)
// constructor
{
	// TO DO: 
	//		- create (initialize) mutex
	//		- create (initialize) condition variables
	//		- set the state of all philosophers to THINKING




}

void DiningPhil_Destroy(DiningPhil *dp)
// destructor
{
	pthread_mutex_destroy(&dp->mutex);
	for(int i = 0; i < 5; i++)
		pthread_cond_destroy(&dp->self[i]);
}

void DiningPhil_PickUp(DiningPhil *dp, int id)
{
#ifdef	_DEBUG
	ExclusivePrintf(id, "PickUp(%d)\n", id);
#endif	//	_DEBUG

	// TO DO: implement the pickUp() procedure for the entry section
	//		- determine whether this function should be mutually exclusive and implement it as such.
	//		- do not make it mutually exclusive unless it is necessary






#ifdef	_DEBUG
	ExclusivePrintf(id, "PickUp(%d) - done.\n", id);
#endif	//	_DEBUG
}

void DiningPhil_PutDown(DiningPhil *dp, int id)
{
#ifdef	_DEBUG
	ExclusivePrintf(id, "PutDown(%d)\n", id);
#endif	//	_DEBUG

	// TO DO: implement the putDown() procedure for the entry section
	//		- determine whether this function should be mutually exclusive and implement it as such.
	//		- do not make it mutually exclusive unless it is necessary
	






#ifdef	_DEBUG
	ExclusivePrintf(id, "PutDown(%d) - done.\n", id);
#endif	//	_DEBUG
}

void DiningPhil_Test(DiningPhil *dp, int id, int caller)
// the implementation of the test() procedure.
// the 'caller' argument is the index of the philosopher that called this function
// e.g., Philosoper 2 can call this function as 'DiningPhil_Test(dp, 1, 2);' to test Philosopher 1.
// e.g., Philosoper 2 can call this function as 'DiningPhil_Test(dp, 2, 2);' to test her/himself.
{
#ifdef	_DEBUG
	ExclusivePrintf(caller, "Test(%d)\n", id);
#endif	//	_DEBUG

	// TO DO: implement the test() procedure for the entry section
	//		- determine whether this function should be mutually exclusive and implement it as such.
	//		- do not make it mutually exclusive unless it is necessary







#ifdef	_DEBUG
	ExclusivePrintf(caller, "Test(%d) - done.\n", id);
#endif	//	_DEBUG
}

void DiningPhil_Check(DiningPhil *dp)
// check for the violation of the rule that adjacent philosophers cannot eat at the same time.
// DO NOT modify this function
{
	int prev = 4;
	for(int i = 0; i < 5; i++){
		if(dp->state[prev] == EATING && dp->state[i] == EATING){
			printf("ERROR! Philosopher %d and %d are eating at the same time!\n", prev, i);
			exit(-1);
		}
		prev = i;
	}
}

// for the tread functions 
typedef struct {
	DiningPhil *dp;
	int idx;
} ThreadParam;

int gContinue = TRUE;

void* Philosopher(void *arg);		// thread function


int main(int argc, char *argv[])
// DO NOT modify this function
{
	if(argc == 1){
		printf("Usage: %s <duration>\n", argv[0]);
		return 0;
	}

	int duration = atoi(argv[1]);
	printf("duration = %d\n", duration);

	srand(time(NULL));

	DiningPhil dp = { 0 };
	DiningPhil_Init(&dp);

	pthread_t tid[5] = { 0 };
	ThreadParam param[5] = { 0 };

	// prepare the parameters for the philosopher threads
	for(int i = 0; i < 5; i++){
		param[i].idx = i;
		param[i].dp = &dp;
		
		pthread_create(&tid[i], NULL, Philosopher, &param[i]);
	}

	// waiting for the specified duartion checking for violation every second
	for(int i = 0; i < duration; i++){
		sleep(1);
		DiningPhil_Check(&dp);
	}

	printf("Terminating threads \n");
	gContinue = FALSE;

	// release child threads from the waiting on the condition variables
	for(int i = 0; i < 5; i++)
		pthread_cond_signal(&dp.self[i]);

	printf("Waiting for threads \n");
	for(int i = 0; i < 5; i++)
		pthread_join(tid[i], NULL);

	printf("Destroying dp...\n");
	DiningPhil_Destroy(&dp);
	
	printf("Bye!\n");				// this message must be displayed

	return 0;
}

void* Philosopher(void *arg)
// the thread function for each philosopher
// DO NOT modify this function
{
	ThreadParam *param = (ThreadParam*)arg;

	while(gContinue){
		ExclusivePrintf(param->idx, "%d) Thinking\n", param->idx);
		sleep(rand() % 3 + 1);		// wait for 1~3 seconds

		DiningPhil_PickUp(param->dp, param->idx);

		ExclusivePrintf(param->idx, "%d) Eating\n", param->idx);
		sleep(rand() % 3 + 1);		// wait for 1~3 seconds

		DiningPhil_PutDown(param->dp, param->idx);
	}

	ExclusivePrintf(param->idx, "%d) Terminating\n", param->idx);

	return NULL;
}

// mutex for display routines
pthread_mutex_t display_mutex = PTHREAD_MUTEX_INITIALIZER;

void ExclusivePrintf(int indent, const char *format, ...)
// DO NOT modify this function
{
	va_list vl;

	pthread_mutex_lock(&display_mutex);

	for(int i = 0; i < indent; i++)
		putchar('\t');

	va_start(vl, format);
	vprintf(format, vl);
	va_end(vl);

	fflush(stdout);

	pthread_mutex_unlock(&display_mutex);
}


// end of file <problem for section 02>