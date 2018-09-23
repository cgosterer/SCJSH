#include <libgen.h>
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

void afterregexec();
void afterbgexec();
void freeq();

void myexec(char ** cmd, bool hasio, bool isbg, int input, int output)				// this will exec a given command given the proper path location,
{
        int status;
	int i;
	pid_t pid = fork();

	if (pid == -1)					// error forking, this has never occured yet
	{
		exit(1);
	}

	else if (pid == 0)				// inside child
	{
		if(input != 0)
			dup2(input,STDIN_FILENO);				// stdin_fileno part of unistd should have stdin fileno in it

		if(output != 0)
			dup2(output,STDOUT_FILENO);
		
		if(strcmp(cmd[0], "echo") == 0)
		  {
		    //printf("Echoing\n");
		    for(i = 1; cmd[i] != NULL; i++)
		      {
			printf("%s ", cmd[i]);
		      }
		    printf("\n");
		    exit(0);
		  }

		//printf("Executing %s with input : %d and output : %d\n", cmd[0], input, output);
		execv(cmd[0], cmd);
		printf("problem executing %s\n", cmd[0]);
		exit(1);
	}

	else									// inside parent
	{
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
				//free(queue);
				freeq();
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
			//return;
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
				//free(queue);
				freeq();
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
			//return;
		}

		else if( hasio == false && isbg == false)					// regular command ?
		  {
			waitpid(pid, &status, 0);
      			//printf("%s has finished waiting on it's child\n", cmd[0]);
		  }
	}

	if (isbg == true)
		afterbgexec();
	if (isbg == false)
		afterregexec();
}

void afterbgexec()
{
				int q, status;
                                for (q=1;q <= poscounter; q++)
                                {
                                        if (    waitpid( queue[q].pid, &status, WNOHANG) != 0   )               // was WNOHANG
                                        {
                                                if( queue[q].cmd[0] != NULL && queue[q].printed == false)
                                                {
                                                        printf("[%d]+\t[", q);
                                                        int i = 0;
                                                        while(queue[q].cmd[i] != NULL)
                                                        {
								if (i == 0)
								{
									char * filename = (char*) malloc( strlen(queue[q].cmd[i] + 1 * sizeof(char) ));
									strcpy(filename, basename(queue[q].cmd[i]));
									printf("%s ", filename);
									free(filename);
									i++;
									continue;
								}

                                                                printf("%s ", queue[q].cmd[i]);
                                                                i++;
                                                        }
                                                        printf("]\n");

                                                        queue[q].printed = true;

                                                        bool donequeue = true;
                                                        int x;
                                                        if(poscounter >= 1)
                                                        for (x = 1; x <= poscounter; x++)
                                                        {
                                                                if(queue[x].printed == false)
                                                                        donequeue = false;
                                                        }

                                                        if(donequeue == true)
                                                        {
                                                                //free(queue);
								freeq();
                                                                queue = malloc(100 * sizeof(process));
                                                                poscounter = 0;
                                                        }
                                                }
                                        }
                                }
}															// end afterbgexec

void afterregexec()
{
				int q, status;
                                for (q=1;q <= poscounter; q++)
                                {
                                        if (    waitpid( queue[q].pid, &status, WNOHANG) != 0   )               // was WNOHANG
                                        {
                                                if( queue[q].cmd != NULL && queue[q].printed == false)
                                                {
                                                        printf("[%d]+\t[", q);
                                                        int i = 0;
                                                        while(queue[q].cmd[i] != NULL)
                                                        {
								if (i == 0)
                                                                {
                                                                        char * filename = (char*)malloc(strlen(queue[q].cmd[i] + 1 * sizeof(char)));
                                                                        strcpy(filename, basename(queue[q].cmd[i]));
                                                                        printf("%s ", filename);
                                                                        free(filename);
                                                                        i++;
                                                                        continue;
                                                                }
                                                                printf("%s ", queue[q].cmd[i]);
                                                                i++;
                                                        }
                                                        printf("]\n");
                                                        queue[q].printed = true;
                                                }
                                        }
                                }
}

void freeq()                                                    // free memory from the queue
{
                int i;
                for (i = 1; i <= poscounter; i++)
                {
                        int x = 0;
                        while(queue[i].cmd[x] != NULL )
                        {
                                free(queue[i].cmd[x]);
                                x++;
                        }
                }
                free(queue);
}
