#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


void myexecio(char ** cmd, bool hasio)	// bool hasio     this will exec a given command given the proper path location, typically in bin/<cmd>
{

	char pfilename[30];		// proc filename
	char stats[150];		// the stats from the proc file
	int status;
	pid_t pid = fork();

	if (pid == -1)
	{
		exit(1);							// error starting child process
	}
	else if (pid == 0)
	{
		execv(cmd[0], cmd);
		printf("problem executing %s\n", cmd[0]);			// io code here?
		exit(1);
	}
	else
	{
		sprintf(pfilename, "/proc/%d/io", pid);				// x was pid
		FILE * readfile = fopen(pfilename, "r");

		 //waitpid(pid, &status, 0);

		if(hasio == true)
		{
			fscanf(readfile, "%s", stats);
			printf("\t\t%s", stats);
			fscanf(readfile, "%s", stats);
                        printf("\t\t\t%s", stats);
			printf("\n");

			fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t\t\t%s", stats);
                        printf("\n");

			fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t\t\t%s", stats);
                        printf("\n");

			fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t\t\t%s", stats);
                        printf("\n");

			fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        printf("\n");

			fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        printf("\n");

		 	fscanf(readfile, "%s", stats);
                        printf("\t\t%s", stats);
                        fscanf(readfile, "%s", stats);
                        printf("\t%s", stats);
                        printf("\n");

			waitpid(pid, &status, 0);		// -1 was pid
		}						// end hasio == true if statement

		//wait(0);
		waitpid(pid, &status, 0);			// -1 was pid
		return;
	}
}

