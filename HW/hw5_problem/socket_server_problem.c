/*
	socket_server.c: open a port and converts digit strings into the corresponding alphabet string
		communicates with the client through SOCKET
		e.g., "135246" --> "one three five two four six"

		try to make the output as similar to the following example as possible

	Example) ./socket_server 1234		// server-port = 1234
		port = 1234
		creating socket...
		Done.
		socket binding...
		Done.
		listening socket...
		Done.
		Waiting for connect request...
		Connected.
		input = [1234]					// received "1234" from the client
		output = [one two three four]
		Sending message...				// sending "one two three four" to the client
		Done.
		input = [872]
		output = [eight seven two]
		Sending message...
		Done.
		input = [1004]
		output = [one zero zero four]
		Sending message...
		Done.
		input = [1829401]
		output = [one eight two nine four zero one]
		Sending message...
		Done.
		Closing sockets					// received "quit"
		Done.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// converts a digit string into the corresponding alphabet string
void Digit2Text(char *digit_string, char *alphabet_str);

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s <server-port>\n", argv[0]);
		return 0;
	}

	int ret = 0;
	int port = atoi(argv[1]);
	printf("port = %d\n", port);

	printf("creating socket...\n");
	// TO DO: create a socket (server socket) using the port number. store the socket in an integer variable
	//	on failure, display an error message and terminate




	printf("Done.\n");

	struct sockaddr_in svr_addr = { 0 };
	// TO DO: fill svr_addr with appropriate values





	printf("socket binding...\n");
	// TO DO: bind the socket to svr_addr
	//	on failure, display an error message and terminate





	printf("Done.\n");


	printf("listening socket...\n");
	// TO DO: listen the server socket
	//	on failure, display an error message and terminate





	printf("Done.\n");

	struct sockaddr_in client_addr = { 0 };
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_addr_len = 0;

	printf("Waiting for connect request...\n");
	// TO DO: accept a connection request by calling acctp(). keep the client socket in an integer variable
	//	on failure, display an error message and terminate





	printf("Connected.\n");

	char input[64] = "";
	char output[512] = "";

	/* TO DO: repeat
		read an input string from the socket using read()
		if the input is "quit" or an empty string, break
		convert the input into the corresponding alphabet string by calling Digit2Text()
		write the alphabet string into the socket using write()
	*/







	printf("Closing sockets\n");

	// TO DO: close the two sockets





	printf("Done.\n");

	printf("Bye!\n");

	return 0;
}

void Digit2Text(char *digit_str, char *alphabet_str)
{
	static char digit_name[][16] = {
		"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
	};

	// TO DO: convert a digit string into the corresponding alphabet string
	//	e.g., "135246" --> "one three five two four six"
	// 	if digit_string is an empty string or contains a non-digit character
	//		set alphabet_str to "" and return






}

