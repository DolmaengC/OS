/*
	socket_client.c: reads digit strings from the user
		and converts it into the corresponding alphabet string using socket_server.
		communicate with socket_client through SOCKET
		e.g., "135246" --> "one three five two four six"

		try to make the output as similar to the following example as possible

	Example) ./socket_client 127.0.0.1 1234 4321
		Connecting to 127.0.0.1:1234
		Done.
		Input a number: 1234
		1234 ==> one two three four
		Input a number: 872
		872 ==> eight seven two
		Input a number: 1004
		1004 ==> one zero zero four
		Input a number: 1829401
		1829401 ==> one eight two nine four zero one
		Input a number: quit
		quit ==>
		Closing sockets
		Done.


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 256

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s <server-ip> <server-port> <client-port>\n", argv[0]);
		return -1;
	}

	char *server_ip = argv[1];
	int server_port = atoi(argv[2]);

	// TO DO: create a socket
	//  on failure, display an error message and terminate
	int client_socket = socket (PF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Failed socket!\n");
		exit(1);
	}






	printf("Connecting to %s:%d\n", server_ip, server_port);

	struct sockaddr_in svr_addr = { 0 };
	// TO DO: set svr_addr with appropriate values
	memset( &svr_addr, 0, sizeof( svr_addr ));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port = htons( server_port );
	svr_addr.sin_addr.s_addr = inet_addr( server_ip );




	// TO DO: connect to svr_addr using the socket
	//  on failure, display an error message and terminate
	if ( connect( client_socket, (struct sockaddr*)&svr_addr, sizeof( svr_addr ) ) == -1) {
		printf("Failed connect!\n");
		exit(1);
	}



	printf("Done.\n");

	/* TO DO: repeat
		read a string from the user
		write the input string to the socket using write()
		read the conversion result from the socket using read()
		display the input string and the conversion result
		if the conversion result is an empty string, break
	*/
	char input[64] = "";
	char output[512] = "";
	while(1) {
		printf("Input a number: ");
		scanf("%s", input);
		write( client_socket, input, strlen(input) + 1 );
		read( client_socket, output, 512 );
		printf("%s ==> %s\n", input, output);
		if ( strlen(output) == 0 ) break;
	}







	printf("Closing socket\n");

	// TO DO: close the socket
	close( client_socket );


	printf("Done.\n");

	printf("Bye!\n");

	return 0;
}
