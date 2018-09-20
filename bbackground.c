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
        char * cmd;             // the cmd
	bool printed;
}process;

 ///struct process queue[100];
 process * queue;
 int poscounter = 0;


void bbexec(char ** cmd)                                // this will exec a given command given the proper path loc$
{
	int cmdsize = 0;
	int i = 0;
	while ( cmd[i] != NULL)
	{
		cmdsize++;
		i++;
	}

        int status;
        pid_t pid = fork();

        if (pid == -1)
        {
                exit(1);
        }

        else if (pid == 0)
        {
                execv(cmd[0], cmd);
                printf("problem executing %s\n", cmd[0]);
                exit(1);
        }

        else
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
		queue[poscounter].cmd = (char*) malloc(sizeof(char) * strlen(cmd[0]));
		strcpy(queue[poscounter].cmd, cmd[0]);
                waitpid(pid, &status, WNOHANG);
		//waitpid(pid, &status, 0);
	}
}

/*
int main()
{
        char * cmd1[4] = {"/bin/sleep", "5", NULL}; // must adjust parser to get command location in the PATH
        char * cmd2[2] = {"/bin/pwd", NULL};            // also must count number of flags arguments so we know how$
        execbg(cmd1);
	execbg(cmd2);
        return 0;
}
*/

