#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


int main()
{
	char cwd[100];

	getcwd(cwd, sizeof cwd);
	printf("the cwd is %s\n", cwd);
	chdir("..");

	getcwd(cwd, sizeof cwd);
	printf("the cwd is %s\n", cwd);

	char * cmd[2] = {"/bin/ls", NULL};

	int status;

	pid_t pid = fork();

	if (pid == -1)
		exit(1);

	else if (pid == 0)
	{
		execv(cmd[0], cmd);
		exit(1);
	}

	else
		waitpid(pid, &status, 0);

	return 0;
}
