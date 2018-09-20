#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct process
{
        int position;           // position in queue
        int pid;                // should be pid_t
	char ** cmd;		// the processes cmd along with its flags
	bool printed;
}process;

process * queue;		// the queue, allocated in parser.c main func
int poscounter = 0;		// position in the queue


void myexec(char ** cmd, bool hasio, bool isbg, int input, int output)				// this will exec a given command given the proper path location,
{
	int status;
	pid_t pid = fork();

	if (pid == -1)					// error forking, this has never occured yet
	{
		exit(1);
	}

	else if (pid == 0)				// inside child
	{
		execv(cmd[0], cmd);
		printf("problem executing %s\n", cmd[0]);
		exit(1);
	}

	else									// inside parent
	{
		if(input != 0)
			dup2(input,STDIN_FILENO);				// stdin_fileno part of unistd should have stdin fileno in it

		if(output != 0)
			dup2(output,STDOUT_FILENO);

		if(hasio == true && isbg == true)				// attempt at combining background and io commands
		{
			char pfilename[30];		// proc filename
			char stats[150];

			sprintf(pfilename, "/proc/%d/io", pid);				// x was pid
			FILE * readfile = fopen(pfilename, "r");

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

			int status,x;
			bool donequeue = true;

			if(poscounter >= 1)
			for (x = 1; x <= poscounter; x++)
			{
				if(queue[x].printed == false)
					donequeue = false;
			}

			if(donequeue == true)
			{
				free(queue);
				queue = malloc(100 * sizeof(process));
				poscounter = 0;
			}

			poscounter++;
			printf("[%d]\t[%d]\n", poscounter, pid);
			queue[poscounter].printed = false;
			queue[poscounter].position = poscounter;
			queue[poscounter].pid = pid;

			int cmdsize = 0;
                        int i = 0;
                        while(cmd[i] != NULL)
                        {
                                cmdsize++;
                                i++;
                        }

                        queue[poscounter].cmd = (char**)malloc( (cmdsize + 1) * sizeof(char*));

                        for (i = 0; i < cmdsize; i++)
                        {
                                queue[poscounter].cmd[i] = (char*) malloc(sizeof(char) * strlen(cmd[i]) + 1);
                                strcpy(queue[poscounter].cmd[i], cmd[i]);
                        }
                        queue[poscounter].cmd[cmdsize] = NULL;

                	waitpid(pid, &status, WNOHANG);
		}

		if (hasio == true && isbg == false)
		{	int status;
			char pfilename[30];					// proc filename
			char stats[150];					// the stats from the proc file

			sprintf(pfilename, "/proc/%d/io", pid);				// x was pid
			FILE * readfile = fopen(pfilename, "r");

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

			waitpid(pid, &status, 0);						// -1 was pid
			return;
		}

		if (isbg == true && hasio == false)						// if it is only a bg process
		{
			int status,x;
			bool donequeue = true;

			if(poscounter >= 1)
			for (x = 1; x <= poscounter; x++)
			{
				if(queue[x].printed == false)
					donequeue = false;
			}

			if(donequeue == true)
			{
				free(queue);
				queue = malloc(100 * sizeof(process));
				poscounter = 0;
			}

			poscounter++;
			printf("[%d]\t[%d]\n", poscounter, pid);
			queue[poscounter].printed = false;
			queue[poscounter].position = poscounter;
			queue[poscounter].pid = pid;

			int cmdsize = 0;
                        int i = 0;
                        while(cmd[i] != NULL)
                        {
                                cmdsize++;
                                i++;
                        }

			queue[poscounter].cmd = (char**)malloc( (cmdsize + 1) * sizeof(char*));

                        for (i = 0; i < cmdsize; i++)
                        {
                                queue[poscounter].cmd[i] = (char*) malloc(sizeof(char) * strlen(cmd[i]) + 1);
                                strcpy(queue[poscounter].cmd[i], cmd[i]);
                        }
			queue[poscounter].cmd[cmdsize] = NULL;

               	 	waitpid(pid, &status, WNOHANG);
			return;
		}

		else if( hasio == false && isbg == false)					// regular command ?
			waitpid(pid, &status, 0);

	}
}

/*
int main()
{
	char * cmd1[4] = {"/bin/ls", "-l", "-a", NULL};	// must adjust parser to get command location in the PATH
	char * cmd2[2] = {"/bin/pwd", NULL};		// also must count number of flags arguments so we know how big to make char * cmdx
	myexec(cmd2, 1,1,0,0);
	return 0;
}

*/
