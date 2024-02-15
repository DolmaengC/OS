/*
	
	Extend your solution to Problem 2 to prevent the user from moving out of the working directory
	Upgrade SplitCommandLine() to SplitCommandLine2().
		- if the command contains an argument containing a directory, remove the directory name from the argument.
		- if an argument contains "..", replace it by "."

	Example)
		Welcome to myshell.							// greeting message
		All commands will be executed in "Working/".
		Type "quit" to quit.
		$ ls -al									// 'ls -al' command
		argc = 2
		argv[0] = ls
		argv[1] = -al
		argv[2] = (null)
		total 0										// the working directory contains a text file 'test.txt'
		drwxrwxrwx 1 callee callee 512 Mar 25 09:55 .
		drwxrwxrwx 1 callee callee 512 Mar 25 10:18 ..
		-rwxrwxrwx 1 callee callee   6 Mar 25 09:55 test.txt
		$ ls ..										// the user tries to see the parent directory
		argc = 2
		argv[0] = ls
		argv[1] = .									// ".." was replaced by "."
		argv[2] = (null)
		test.txt									// display only the current directory
		$ cp test.txt ..							// the user tries to copy 'test.txt' to the parent directory
		argc = 3				
		argv[0] = cp
		argv[1] = test.txt
		argv[2] = .									// ".." was replaced by "."
		argv[3] = (null)
		cp: 'test.txt' and './test.txt' are the same file	// the shell executed 'cp test.txt .' 
		$ vi ../test2.txt							// the user tries to create a file 'test2.txt' in the parent directory
		argc = 2
		argv[0] = vi
		argv[1] = test2.txt							// "../test2.txt" was replaced by "test2.txt"
		argv[2] = (null)
		// the shell execute 'vi test2.txt'. the new file is created in the working directory
		$ ls
		argc = 1
		argv[0] = ls
		argv[1] = (null)
		test.txt  test2.txt 						// the current directory contains 'test2.txt'
		$ quit
		Bye!


	#################################################
	# The followings are the description of Problem 2

	Write a shell (command line interpreter) that runs UNIX commands in a working directory following the instructions
		at startup, move into the working directory (the global variable working_dir).
			if the working directory does not exist, create it by calling the CheckAndCreateDirectory() function
		repeat until the user inputs "quit" command
			read a command line
			split the command line into arguments by calling the SplitCommandLine() function
			for debug, display the split arguments
			if the command line is not empty,
				execute the command line using the fork(), execvp(), and wait() system calls
				// all commands should run in the working directory
			
	- Complete main() following the instructions
	- Write CheckAndCreateDirectory() by reusing the solution to the previous homework with slight modification
	- Write SplitCommandLine() following the instructions


	In the following example, the texts after $ are the commands from the user
	Example)
		Welcome to myshell.							// greeting messages
		All commands will be executed in "Working/".
		Type "quit" to quit.
		Directory Working was created.				// this message was printed because the working directory did not exist
		$ ls										// the first command from the user
		argc = 1									// split arguments
		argv[0] = ls
		argv[1] = (null)
		// the first 'ls' command does not print anything bcause the working directory is initially empty
		// if the working directory already exists and contains files, the 'ls' command should display its contents
		$ ls -al									// the second command from the user
		argc = 2									// split arguments
		argv[0] = ls
		argv[1] = -al
		argv[2] = (null)
		total 0
		// the result of 'ls -al' showing only the current and parent directories
		drwxrwxrwx 1 callee callee 512 Mar 25 09:49 .
		drwxrwxrwx 1 callee callee 512 Mar 25 09:49 ..
		$ vi test.txt								// the third command from the user
		argc = 2
		argv[0] = vi
		argv[1] = test.txt
		argv[2] = (null)
		// the shell executes vi to allow the user to edit 'test.txt'
		$ ls										// the user typed 'ls' again
		argc = 1
		argv[0] = ls
		argv[1] = (null)
		test.txt									// the result of 'ls'. now, it contains a file
		$ quit										// command to terminate the shell
		Bye!										// a goodbye message

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>

#define _DEBUG			// do not disable this line

#define MAX_PATH 256

#define TRUE 1
#define FALSE 0

int CheckAndCreateDirectory(char *file_path);
int SplitCommandLine2(char *cmd, char *argv[]);

char working_dir[256] = "Working/";

int main()
{
	printf("Welcome to myshell.\n");
	printf("All commands will be executed in \"%s\".\n", working_dir);
	printf("Type \"quit\" to quit.\n");

	int ret = CheckAndCreateDirectory(working_dir);
	if(ret == FALSE){
		printf("Failed to find or create directory \"%s\".\n", working_dir);
		return -1;
	}

	// TO DO: move into the working directory (call chdir())
	//      on failure, display an error message
	if(chdir(working_dir) == -1) {
		printf("ERROR! Faied to move %s\n", working_dir);
		return 0;
	}


	while(1){
		char cmd[256] = "";
		int argc = 0;
		char *argv[128];

		// read a command line
		printf("$ ");
		fgets(cmd, 256, stdin);
		cmd[strlen(cmd) - 1] = 0;

		// if the command is "quit", break the loop
		if(strcmp(cmd, "quit") == 0)
			break;

		argc = SplitCommandLine2(cmd, argv);

#ifdef	_DEBUG
		printf("argc = %d\n", argc);
		for(int i = 0; i < argc; i++)
			printf("argv[%d] = %s\n", i, argv[i]);
		printf("argv[%d] = %s\n", argc, argv[argc]);
#endif	//	_DEBUG

		// TO DO: if argc is greater than zero, run the command using fork() and execvp()
		// 		on failure of fork() or execvp(), display an error message
		//		for safety, call exit(-1) after execvp() in the child process
		//		the parent should wait for the child to terminate
		if(argc > 0) {
			int child = fork();
			if(child > 0) {
				wait(NULL);
			}
			else if(child == 0) {
				if(execvp(argv[0], argv) == -1) {
					printf("ERROR! Failed execvp.\n");
					return 0;
				}
				else {
					exit(-1);
				}
			}
			else {
				printf("ERROR! Failed fork.\n");
				return 0;
			}
		}

	}

	printf("Bye!\n");			// this message MUST be displayed

	return 0;
}


int CheckAndCreateDirectory(char *file_path)
// TO DO: check if the desination directory exists. otherwise, create the destination directory.
// 		if the destination directory exists or successfully created, return TRUE
//		otherwise, return FALSE
//		reuse your solution to the presvious homework with slight modification
{
	
	//  Algorithm)
	// 	repeat for i from 0 to the length of file_path
	//		if file_path[i] is a delimitor ('/'),
	//			make a subdirectory path as a string with the characters from file_path[0] to file_path[i].
	//		 		(don't foreget to add '\0' at the end of the string)
	//			if the subdirectory exists,			// do not check whether the directory is writable,
	//				display a message indicating the subdirectory exists.
	//			otherwise,
	//				create the subdirectory.		// use mkdir() with mode 0755		// 0766 --> 0755
	//				on success,
	//					// display a message indicating the subdirectory is created		// skip
	//				otherwise,
	//					display an error message and return FALSE
	//
	//  return TRUE
	static char subdir[MAX_PATH] = "";
	int access_mode = F_OK;
	mode_t mkdir_mode = 0755;
	for(int i = 0; i < strlen(file_path); i++) {
    	if(file_path[i] == '/') {
        	subdir[0] = '\0';
            strncat(subdir, file_path, i);
            subdir[i] = '\0';
            if(access(subdir, access_mode) == 0) {
                //printf("Found directory %s.\n", subdir);
            }
            else {
                if(mkdir(subdir, mkdir_mode) != 0) {
                    printf("ERROR! Failed make directory %s.\n", subdir);
                    return FALSE;
                }
                printf("Directory %s was created.\n", subdir);
            }
        }
    }
	return TRUE;
}

int SplitCommandLine2(char *cmd, char *argv[])
// TO DO: cut the command into arguments
//		return the number of arguments (argc)
//		set argv[argc] to NULL 
//		- if an argument starts with a quote ('\"' or '\''), extract all characters, including whitespaces, upto the closing quote.
//		- if an argument contains directories, discard directory names (e.g., "/home/user/test.c" --> "test.c")
//		- if an argument is "..", replace it with "."
//
// example) cp test.c test2.c
//		argc = 3
//		argv[0] = cp
//		argv[1] = test.c
//		argv[2] = test2.c
//		argv[3] = (null)			// (nil) indicates the NULL pointer
//
// example) echo "God bless you!"	// echo 'God bless you!' should produce the same result
//		argc = 2
//		argv[0] = echo
//		argv[1] = God bless you!
//		argv[2] = (null)
//
// example) cp test.c ../test2.c
//		argc = 3
//		argv[0] = cp
//		argv[1] = test.c
//		argv[2] = test2.c			// directory name was discarded
//		argv[3] = (null)
{
//	Algorithm
//		set argc to zero
//		repeat for i from 0 to the end of cmd
//			if cmd[i] is a quote ('\"' or '\'')
//				increase i to bypass the quote
//				set argv[argc] to (cmd + i)	to store the starting address of a new argument into the argument array
//				increase i until find the null character ('\0') or the closing quote
//					if cmd[i] is '/', set argv[argc] to (cmd + i + 1)		// modified
//				increase argc
//				if cmd[i] is the null character, decrease i to make the condition of the for-loop fail	// modified
//				otherwise, set cmd[i] to zero to make the current argument a string
//				if the new argument is "..", replace it by "."				// modified
//			otherwise, if cmd[i] is not a whitespace,			// use isspace() declared in ctype.h
//				set argv[argc] to (cmd + i)	to store the starting address of a new argument into the argument array
//				increase i until find the null character ('\0') or a whitespace
//					if cmd[i] is '/', set argv[argc] to (cmd + i + 1)		// modified
//				increase argc
//				if cmd[i] is the null character, decrease i to make the condition of the for-loop fail	// modified
//				otherwise, set cmd[i] to zero to make the current argument a string
//				if the new argument is "..", replace it by "."				// modified
//		set argv[argc] to NULL
//		return argc
//
	
	int argc = 0;
	int len = strlen(cmd);
	for(int i = 0; i < len; i++) {
		if(cmd[i] == '\'' || cmd[i] == '\"') {
			i++;
			argv[argc] = cmd + i;
			while(cmd[i] != '\0' && cmd[i] != '\"' && cmd[i] != '\'') {
				i++;
				if(cmd[i] == '/') {
					argv[argc] = cmd + i + 1;
				}
			}
			argc++;
			if(cmd[i] == '\0') {
				// previous code
				//break; // break -> i--

				// updated code 
				i--;
			}
			else {
				cmd[i] = 0;
			}
			if(strcmp(argv[argc - 1], "..") == 0) {
				argv[1] = 0;
			}
		}
		else if (!isspace(cmd[i])) {
			argv[argc] = cmd + i;
			while(!isspace(cmd[i]) || cmd[i] == '\0') {
				i++;
				if(cmd[i] == '/') {
					argv[argc] = cmd + i + 1;
				}
			}
			argc++;
			if(cmd[i] == '\0') {
				i--;
			}
			else {
				cmd[i] = 0;
			}
			if(strcmp(argv[argc - 1], "..") == 0) {
				argv[1] = 0;
			}

		}
	}
	argv[argc] = NULL;

	return argc;
}

