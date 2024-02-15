/*
	
	Write a C program that copies a file.
	* Read the source and destination pathnames from the command line arguments.
	* If the destination path includes directories that do not exist, create the directories.
	* After copying the file, run 'ls -al <dest_path>' by calling the system() function.

	Make the output as close to the example as possible.

	Example) 
		$ cat test.txt							# display the contenets of a file 'test.txt'.
		This is a test file!
		
		$ ./hw2_3 test.txt NewDir/test2.txt		# initially, directory NewDir does not exist.
		3 arguments:							# display command line arguments.
		argv[0] = ./hw2_3
		argv[1] = test.txt
		argv[2] = NewDir/test2.txt
		Preparing directory...
		Directory NewDir was created.			# create NewDir since it does not exist.
		result = 1
		Copying file test.txt to NewDir/test2.txt
		src = 3
		dest = 4
		read_size = 22, written_size = 22		# read and wrote 22 bytes 
		read_size = 0, written_size = 22		# zero byte was read. written_size contains the previous value
		test.txt was successfully copied to NewDir/test2.txt
		-rw-r--r-- 1 user user 22  3월 11 16:46 NewDir/test2.txt
		
		$ ./hw2_3 test.txt NewDir/NewDir2/test3.txt  # this time, NewDir exists but NewDir/NewDir2 does not.
		3 arguments:							# display command line arguments
		argv[0] = ./hw2_3
		argv[1] = test.txt
		argv[2] = NewDir/NewDir2/test3.txt
		Preparing directory...
		Found directory NewDir.					# NewDir was already created by the previous execution
		Directory NewDir/NewDir2 was created.	# create NewDir/NewDir2 since it does not exist.
		result = 1
		Copying file test.txt to NewDir/NewDir2/test3.txt
		src = 3
		dest = 4
		read_size = 22, written_size = 22		# read and wrote 22 bytes 
		read_size = 0, written_size = 22		# zero byte was read. written_size contains the previous value
		test.txt was successfully copied to NewDir/NewDir2/test3.txt
		-rw-r--r-- 1 user user 22  3월 11 16:46 NewDir/NewDir2/test3.txt


	To solve this problem, search the Internet for the following functions and learn how to use them.

    * int open(const char *pathname, int flags, mode_t mode);
	* int close(int fd);
	* ssize_t read (int fd, void *buf, size_t nbytes);
	* ssize_t write (int fd, void *buf, size_t len);
	* int access( const char *path, int mode);
	* int mkdir(const char *pathname, mode_t mode);

	Complete the program by writing two functions CheckAndCreateDirectory() and CopyFile().

	Compilation command:
		gcc hw2_3.c -o hw2_3

	Execution command:
		hw2_3 <src_path> <dest_path>

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 2

#define MAX_PATH 256			// maximum length of pathnames
#define BUFFER_SIZE 512			// the size of buffer to copy files

int CheckAndCreateDirectory(char *file_path);
int CopyFile(char *src_path, char *dest_path);

int main(int argc, char *argv[])
{
	// display the command line arguments
	printf("%d arguments:\n", argc);
	for(int i = 0; i < argc; i++)
		printf("argv[%d] = %s\n", i, argv[i]);


	// if argc < 3, display the usage and terminate.
	if(argc < 3){
		printf("Usage: %s <src_path> <dest_path>\n", argv[0]);
		return 0;
	}

	char *src_path = argv[1];
	char *dest_path = argv[2];

	// test if the source file exists and is readible.
	if(access(src_path, R_OK) != 0){
		printf("Error! File %s does not exist or is not readible!\n", src_path);
		exit(-1);
	}

	// check whether the destination directory exists. otherwise, create the directory
	printf("Preparing directory...\n");
	int success = CheckAndCreateDirectory(dest_path);
	printf("result = %d\n", success);

	// copy file
	printf("Copying file %s to %s\n", src_path, dest_path);;
	if(success)
		success = CopyFile(src_path, dest_path);

	// on success, run 'ls -al <dest_path>' by calling the system() function
	// otherwise, display an error message.
	if(success){
		printf("%s was successfully copied to %s\n", src_path, dest_path);

		char cmd[MAX_PATH] = "";
		sprintf(cmd ,"ls -al %s", dest_path);
		system(cmd);
	} else {
		printf("Failed to copy %s to %s!\n", src_path, dest_path);
	}

	return 0;
}

int CheckAndCreateDirectory(char *file_path)
// TO DO: check if the desination directory exists. otherwise, create the destination directory.
// 		if the destination directory exists or successfully created, return TRUE
//		otherwise, return FALSE
{
	
	//  Algorithm)
	// 	repeat for i from 0 to the length of file_path
	//		if file_path[i] is a delimitor ('/'),
	//			make a subdirectory path as a string with the characters from file_path[0] to file_path[i].
	//		 		(don't foreget to add '\0' at the end of the string)
	//			if the subdirectory exists and is writable,
	//				display a message indicating the subdirectory exists.
	//			otherwise,
	//				create the subdirectory.		// use mkdir() with mode 0766
	//				on success,
	//					display a message indicating the subdirectory is created
	//				otherwise,
	//					display an error message and return FALSE
	//
	//  return TRUE

	static char subdir[MAX_PATH] = "";








	return TRUE;
}

int CopyFile(char *src_path, char *dest_path)
// TO DO: copy src_path to dest_path
//		assume the destination directory exists
{
	int src = open(src_path, O_RDONLY, 0);
	printf("src = %d\n", src);	

	// TO DO: on failure, display an error message and return FALSE




	int dest = open(dest_path, O_WRONLY | O_CREAT, 0644);
	printf("dest = %d\n", dest);

	// TO DO: on failure, display an error message and return FALSE




	static char buffer[BUFFER_SIZE];
	ssize_t read_size = 0, written_size = 0;

	// TO DO:
	// repeat until there is no more byte to read
	//		read maximum BUFFER_SIZE bytes from the source file into the buffer, storing the number of bytes actually read in a variable.
	//		write the buffer into the destination file








	// TO DO: close the source and destination files




	return TRUE;
}

