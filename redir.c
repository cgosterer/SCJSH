#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

void redir(char ** cmd, char * filename, char type)	// this will exec a given command given the proper path location, typically in bin/<cmd>
{
	pid_t pid = fork();
	int status;

	if (pid == 0)
	{
		if(type == 'r')
		{
			int irfile = open(filename, O_RDONLY  );		//  irfile = input redirection file
			dup2(irfile, 0);
			close(irfile);
			execv(cmd[0], cmd);
		}

		if (type == 'w')
		{
			int orfile = open( filename, O_RDWR | O_TRUNC | O_CREAT , S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );	// orfile = output redirection file
			dup2(orfile, 1);
			close(orfile);
			execv(cmd[0], cmd);
		}
	}

	else if (pid == -1)
	{
		exit(1);
		//execv(cmd[0], cmd);
	}

	else if (pid > 0)
		waitpid(pid, &status, 0);
}

/*
int main()
{
	char * cmd1[4] = {"/bin/ls", "-l", "-a", NULL};	// must adjust parser to get command location in the PATH
	char * cmd2[2] = {"/bin/pwd", NULL};		// also must count number of flags arguments so we know how big to make char * cmdx
	char * cmd3[3] = {"/bin/grep", "VT",  NULL};
	redir(cmd2, 'r');
	return 0;
}
*/
