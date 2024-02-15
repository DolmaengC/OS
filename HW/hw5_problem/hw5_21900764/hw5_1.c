/*

	This program reads an integer expression from the user and computes the result using a child process.
	The parent and child processes communicate with each other through shared memory.
	Implement this program following the instructions.
	Append "[parent]" and "[child]" to all messages in Parent() and Child() respectively.

	Example)
`		shmid = 10
		[parent] Input an integer binary expressions(type "quit" to finish.): 5 + 3
		[child] received expression "5 + 3"
		[child] sending result "8"
		[parent] 5 + 3 = 8
		[parent] Input an integer binary expressions(type "quit" to finish.): 7 *2
		[child] received expression "7 *2"
		[child] sending result "14"
		[parent] 7 *2 = 14
		[parent] Input an integer binary expressions(type "quit" to finish.): 9/2
		[child] received expression "9/2"
		[child] sending result "4"
		[parent] 9/2 = 4
		[parent] Input an integer binary expressions(type "quit" to finish.): quit
		[child] received expression "quit"
		[parent] Terminating Parent.
		[child] Terminating Child.


	You can use the following Linux commands to list or remove shared memory blocks
		ipcs				// show infromation on IPC facilities
		ipcrm -m <shm-id>	// delete a shared memory block


	After the program terminates, your program should not leave a shared memory block
	Example)
		$ ipcs
		...
		------ Shared Memory Segments --------
		key        shmid      owner      perms      bytes      nattch     status
		0x00000000 2          lightdm    600        524288     2          dest
		0x00000000 3          lightdm    600        33554432   2          dest

		// no shared memory block created by the user

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define	_DEBUG				// do not disable this macro

#define TRUE 1
#define FALSE 0

#define P2C 0				// parent to child
#define C2P 1				// child to parent

#define MAX_LEN 128

typedef struct {
	int is_loaded;			// is_loaded == TRUE indicates a data is loaded in buffer. otherwise, buffer is not ready
	char buffer[MAX_LEN];	// buffer to store the data
} SharedBuffer;

/*
	Transferring data through SharedBuffer

	sender:
		wait until is_loaded becomes FALSE
		store data in buffer
		set is_loaded to TRUE
		
	receiver:
		wait until is_loaded becomes TRUE
		retrieve data from buffer
		set is_loaded to FALSE

*/


void* Parent(void *arg);
void* Child(void *arg);
int Evaluate(char *expression);

int main(int argc, char *argv[])
{
	// TO DO: allocate an array composed of two SharedBuffer instances in a shared memory block
	//		the size of shared memory block should be sizeof(SharedBuffer) * 2.
	// 		one is for data transfer from the parent to the child.
	// 		the other is for data transfer from the child to the parent.
	//		call shmget() and shmat()
	// 		on failure, display an error message, deallocate the shared memory block if necessary, and quit.
	// 		otherwise, display the id of the shared memory block for debug.
	key_t key = 1234;
	//SharedBuffer shm_buffer[2];
	int shmid = shmget(key, sizeof(SharedBuffer)*2, IPC_CREAT|0666 );
	if(shmid == -1) {
		// previous code
		/*
		 printf("shamt failed\n");
		exit(0);
		*/

		// updated code
		perror("Failed shmget");
		exit(1);
	}	
	SharedBuffer* shm_buffer = (SharedBuffer*)shmat(shmid, NULL, 0);
	if(shm_buffer == (void*)-1){
		// previous code
		/*
		printf("segmat failed\n");
		exit(0);
		*/

		// updated code
		perror("Failed shmat");
		exit(1);
	}
#ifdef _DEBUG
	printf("shmid = %d\n", shmid);
#endif



	// initialize shm_buffer[0] and shm_buffer[1] (set the is_loaded fields to FALSE and the buffer fields to an empty string.)
	shm_buffer[P2C].is_loaded = FALSE;
	shm_buffer[P2C].buffer[0] = 0;
	shm_buffer[C2P].is_loaded = FALSE;
	shm_buffer[C2P].buffer[0] = 0;

	// TO DO: create a child process
	// 		on failure, display an error message, detatch and deallocate the shared memory block, and terminate.
	//		In the child process,
	//			call the Child() function passing the address to the SharedBuffer array.
	//			detatch the shared memory block
	//			call exit(0) for safety
	// 		In the parent process,
	//			call the Parent() function passing the address to the SharedBuffer array.
	//			detatch and destroy the shared memory block
	int pid = fork();
	if (pid < 0) {
		printf("fork failed\n");
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			printf("shmctl failed\n");
			exit(1);
		}
		exit(1);
	}
	if (pid == 0) {
		Child(shm_buffer);
		if (shmdt(shm_buffer) == -1) {
			printf("[child] stmdt failed\n");
			exit(1);
		}
		exit(0);
	}
	else {
		Parent(shm_buffer);
		if (shmdt(shm_buffer) == -1) {
			// previous code
			//printf("[parent]stdmt failed\n");
				
			// updated code
			perror("[parent] Failed shmdt");
			
			exit(1);
		}
		if(shmctl(shmid, IPC_RMID, 0) == -1) {
			printf("shmctl failed\n");
			exit(1);
		}
	}






	system("ipcs");		// run 'icps' to check if the shared memory block was correctly deallocated
						// if your VM does not have icps, install it by running 'sudo apt install util-linux'
	printf("Bye!\n");

	return 0;
}

void* Parent(void *arg)
// TO DO: implement this function
// 		read integer expressions from the user
// 		send the expression to the child process through the shareed memory buffer
//		receive the result from the child through the shared memory buffer
//		display the expression and result
{
	SharedBuffer *shm_buffer = (SharedBuffer *)arg;

	// Algorithm)
	//	repeat until the user types "quit"
	// 		read an integer expression as a string (e.g., "5+3", "7 * 5", etc.) - use fgets()
	// 		if the input is not an empty string,
	// 			send the expression to the child through shm_buffer[P2C]
	// 				wait until the is_loaded field is FALSE
	//				copy the expression into the buffer field
	//				set the is_loaded field to TRUE
	// 			if the input string is "quit", break the loop
	// 			receive the result of the expression from the child through shm_buffer[C2P]
	//				wait until the is_loaded field is TRUE
	//				copy the result from the buffer field
	//				set the is_loaded field to FALSE
	// 			display the expression and the result from the child.
	//	display a message indicating that the parent is terminating.

	char expression[MAX_LEN] = "";
	char result[MAX_LEN] = "";
	while(1) {
		printf("[parent] Input an integer binary expressions(type \"quit\" to finish.):");
		if( fgets(expression, MAX_LEN, stdin) != NULL ) {
			expression[strlen(expression) -1] = '\0';
			//while(shm_buffer[P2C].is_loaded == TRUE);
			sprintf((char *)shm_buffer[P2C].buffer, "%s", expression);
			shm_buffer[P2C].is_loaded = TRUE;
			if(strcmp(expression, "quit") == 0) break;
			while(shm_buffer[C2P].is_loaded == FALSE);
			result[0] = 0;
			strcpy(result, shm_buffer[C2P].buffer);
			shm_buffer[C2P].is_loaded = FALSE;
			printf("[parent] %s = %s\n", expression, result);
		}
	}






	printf("Terminating Parent.\n");
}

void* Child(void *arg)
// TO DO: implement this function
// 		receive an integer expressions from the parent by message passing
//		compute the result of the expression
// 		send the result to the parent process by message passing
{
	SharedBuffer *shm_buffer = (SharedBuffer *)arg;

	// Algorithm)
	// 	repeat until the incomming message is "quit"
	// 		receieve an expression from the parent through the message queue p2c.
	// 			on failure, display an error message.
	// 		for debug, display the incoming message, its index, and the return value of msgrcv().
	// 		if the message is "quit", break the loop
	// 		compute the evaluation in the incoming message
	// 		for debug, display the outgoing message and its index.
	// 		send the result as a string to the parent through the message queue c2p.
	// 			on failure, display an error message.
	//		for debug, display the return value of msgsnd().
	// 		increase the index of outgoing message.
	//
	//	display a message indicating that the child is terminating.

	char expression[MAX_LEN] = "";
	char result[MAX_LEN] = "";
	while(1) {
		while(shm_buffer[P2C].is_loaded == FALSE);
		expression[0] = 0;
		strcpy(expression, shm_buffer[P2C].buffer);
		shm_buffer[P2C].is_loaded = FALSE;
		printf("[child] received expression \"%s\"\n", expression);
		if(strcmp(expression, "quit") == 0) break;
		sprintf(result, "%d", Evaluate(expression));
		while(shm_buffer[C2P].is_loaded == TRUE);
		printf("[child] sending result \"%s\"\n", result);
		shm_buffer[C2P].buffer[0] = 0;
		strcpy(shm_buffer[C2P].buffer, result);
		shm_buffer[C2P].is_loaded = TRUE;

	} 	
	






	printf("Terminating Child.\n");
}

int Evaluate(char *expression)
{
	int op1 = 0, op2 = 0;
	char op = 0;
	int result = 0;

	int ret = sscanf(expression, "%d %c %d", &op1, &op, &op2);
	if(ret == 3){
		switch(op){
		case '+':
			result = op1 + op2;
			break;

		case '-':
			result = op1 - op2;
			break;

		case '*':
			result = op1 * op2;
			break;

		case '/':
			if(op2 != 0)
				result = op1 / op2;
			break;
		}
	}

	return result;
}

