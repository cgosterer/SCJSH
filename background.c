#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

void execbg(char ** cmd)                                // this will exec a given command given the proper path loc$
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
		//printf("else here\n");

	     printf("[0]\t[%d]\n", pid);				// the [1] should be queue number not implemented yet

             while (   waitpid(pid, &status, WNOHANG) == 0	)
		{
			//printf("still waiting\n");	just keep waiting?
			//printf("%d\n", pid);
		}

		if (waitpid(pid, &status, WNOHANG) != 0)
			printf("[0]+\t");
			printf("[");
			int x;
			for (x = 0; x < cmdsize; x++)
			{
				if (x == cmdsize -1)
					printf("%s", cmd[x]);
				else
					printf("%s ", cmd[x]);
			}
			printf("]\n");
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

