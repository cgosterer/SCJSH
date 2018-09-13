#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


void myexec(char ** cmd, bool hasio)	// bool hasio     this will exec a given command given the proper path location, typically in bin/<cmd>
{					// isio means is there an io built in that needs to be handled

	char pfilename[30];		// proc filename
	char stats[150];		// the stats from the proc file
	int status;
	pid_t pid = fork();
	sprintf(pfilename, "/proc/%d/io", pid);

	FILE * readfile = fopen(pfilename, "r");


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
		//waitpid(pid, &status, 0);					// io code here looks like wait needs to be at end

		//if(hasio == true)
			//fgets(stats, 100, readfile);
			//printf("%s", stats);

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

		waitpid(pid, &status, 0);
	}
}

/*
int main()
{
	char * cmd1[4] = { "/bin/ls", "-l", "-a", NULL};	// must adjust parser to get command location in the PATH
	char * cmd2[2] = {"/bin/pwd", NULL};			// also must count number of flags arguments so we know how big to make char * cmdx
	myexec(cmd1, true);

	int n = sizeof(cmd1)/sizeof(cmd1[0]);
	printf("cmd1 has %d elements\n", n);

	return 0;
}
*/
