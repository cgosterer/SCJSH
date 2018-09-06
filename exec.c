#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

void myexec(char ** cmd)				// this will exec a given command given the proper path location, typically in bin/<cmd>
{
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
		waitpid(pid, &status, 0);
}

int main()
{
	char * cmd1[4] = {"/bin/ls", "-l", "-a", NULL};	// must adjust parser to get command location in the PATH
	char * cmd2[2] = {"/bin/pwd", NULL};		// also must count number of flags arguments so we know how big to make char * cmdx
	myexec(cmd2);

	return 0;
}
