/*

	Write a program that displays the files and subdirectories of the current directory.
	Display name, user ID (uid), group ID (gid), size, and modified date as the following example.
	Make the output as close to the example as possible.

	Example)
		$ ./hw2_2
		hw2_2, uid = 1001, gid = 1001, size = 17032, modified date = 2023/03/11
		NewDir, uid = 1001, gid = 1001, size = 4096, modified date = 2023/03/11
		.., uid = 1001, gid = 1001, size = 4096, modified date = 2023/03/08				// parent directory
		test.txt, uid = 1001, gid = 1001, size = 22, modified date = 2023/03/11
		hw2_3.c, uid = 1001, gid = 1001, size = 7121, modified date = 2023/03/11
		hw2_2.c, uid = 1001, gid = 1001, size = 493, modified date = 2023/03/11
		., uid = 1001, gid = 1001, size = 4096, modified date = 2023/03/1001			// current directory
		hw2_3, uid = 1001, gid = 1001, size = 17320, modified date = 2023/03/11

	To solve this problem, search the Internet for the following functions and structures, and learn how to use them.

	Functions:
	    * DIR *opendir(const char *name);
		* int closedir( DIR *dir);
		* struct dirent *readdir(DIR *dir);
		* int stat(const char *path, struct stat *buf);
		* struct tm *localtime(const time_t *timeval);

	Structures:
		* struct DIR
		* struct dirent
		* struct stat
		* struct tm

*/


#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
	DIR *dir = NULL;
	struct dirent *ent = NULL;

	// TO DO: open current directory using opendir()


	// TO DO:
	// 	repeat until there is more more directory entry
	//		read a directory entry using readdir()
	// 		get the file state using stat()
	//		convert the modified time into struct tm
	//		display the name, uid, gid, size, and modified date (year, month, day)






	// TO DO: close directory


	return 0;
}
