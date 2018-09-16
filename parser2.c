#include <errno.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "./env2.c"
#include "./exec_and_io.c"
#include "./redir.c"
#include "./getcmdpath.c"


char** addToken(char ** instr, char * tok, int numTokens);
void printTokens(char** instr, int numTokens);

int main()
{
	bool echocheck = false;						// set to true if echo is seen
        struct timeval start;
	struct timeval end;
	long int timedif;						// time difference between start and end of shell
	int timedummy1 = gettimeofday(&start, NULL);			// timedummy1 only used to make sure return val doesnt mean error

	const int INST_SIZE = 256;
	int has_exit = 0;
	char token[INST_SIZE];          				// holds instruction token
	char ** bucket;         					// array that holds all instruction tokens
	char temp[INST_SIZE];						// used to split instruction tokens containing special characters

	while (has_exit == 0)						// this is the main loop for the shell
	  {
		echocheck = false;

	    prompt();
	    int numI = 0;	                			// number of tokens in an instruction
	    do {              				                // loop reads character sequences  separated by whitespace, can break fom this loop to go to next iteration of the shell
	      scanf( "%s", token);					//check to see for improper first character read in?
	      readEnv(token, INST_SIZE);

		if ( strcmp(token, "exit") == 0)
		{
		  int timedummy2  = gettimeofday(&end, NULL);		// timedummy2 only used to see if error is returned
		  timedif = end.tv_sec - start.tv_sec;
		  has_exit = 1;
		  							//break;
		}

		if ( strcmp(token, "echo") == 0)
                {
			echocheck = true;
			char    *echobuff;
        		size_t  size = 1024;
        		echobuff = malloc(size);
			int x = getchar();				// ger rid of space between echo and echo's output
        		getline(&echobuff, &size, stdin);
			printf("%s", echobuff);
			break;
                }

	      int i;
	      int start;
	      start = 0;
	      for (i = 0; i < strlen(token); i++)
		{
		  if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&')
		    {
		      if (i-start > 0)
			{
			  memcpy(temp, token + start, i - start);
			  temp[i-start] = '\0';
			  bucket = addToken(bucket, temp, numI);
			  numI++;
			}

		      char specialChar[2];
		      specialChar[0] = token[i];
		      specialChar[1] = '\0';
		      bucket = addToken(bucket,specialChar,numI);
		      numI++;
		      start = i + 1;
		    }
		}

	      if (start < strlen(token))
		{
		  memcpy(temp, token + start, strlen(token) - start);
		  temp[i-start] = '\0';
		  bucket = addToken(bucket, temp, numI);
		  numI++;
		}

	    }while('\n' != getchar());     				//until end of line is reached, end do while loop

	    if (echocheck == false)
	    {
	    	printTokens(bucket, numI);				// print the tokens
		//printf("numI is %d\n", numI);				// print number of tokens

		if ( strcmp(bucket[0],"<")== 0 || strcmp(bucket[0], ">") == 0 ||strcmp(bucket[numI - 1], "<") == 0 || strcmp(bucket[numI - 1], ">") == 0 )
		{
			printf("Syntax error near unexpected token `newline'\n");
			continue;
		}

		if ( strcmp(bucket[0],"|") == 0 || strcmp(bucket[numI - 1], "|") == 0  )
		{
                        printf("Syntax error near unexpected token `|'\n");
			continue;
		}

		int q;
		for(q = 0; q < numI - 1; q++)
		{
			if(	strcmp(bucket[q],"&") == 0 && strcmp(bucket[q+1],"|") == 0					)
				printf("Imporper combination of processing operator\n");

			if(   (  strcmp(bucket[q],"<") == 0 || strcmp(bucket[q],">") == 0 ) && strcmp(bucket[q+1],"&") == 0 )
                                printf("Imporper combination of  processing operator\n");
		}

		if(strcmp(bucket[0], "io") == 0)
		{
			pid_t pid;
			char  pfile[40];				// process file name/id
			char out[300];					// output of proc file
			sprintf(pfile, "/proc/%d/io", pid);
			FILE * procfile = fopen(pfile, "r");

		}

		if (strcmp(bucket[0], "cd") == 0)			// if it is a cd command change cwd of this process (our program parser2.c)
		{
			if (numI == 1)
				chdir(getenv("HOME"));
			if (numI == 2)
			{
				DIR * d = opendir(bucket[1]);
				if(d)
				{
					chdir(bucket[1]);
					long int k = pathconf(".", _PC_PATH_MAX) * sizeof(char);
					char * buff = (char *) malloc(k);
					setenv("PWD", getcwd(buff, k), 1);
	  				free(buff);
				}

				else if (ENOENT == errno)
					printf("%s: No such file or directory.\n", bucket[1]);
			}
			if (numI > 2)
				printf("cd: Too many arguments.\n");
		}

		if(numI > 2 && ( strcmp(bucket[numI - 2],"<")== 0  || strcmp(bucket[numI - 2],">") == 0 )  )	// start executing commands, deal with redir and piping here?
		{
			int i;
			char ** leftof = malloc( (numI - 1) * sizeof(char*) );

			for(i = 0; i < numI - 2; i++)
			{
				leftof[i] = bucket[i];
				printf("the string copied is: %s\n", leftof[i]);				// left of now has {"ls","-l","-a", NULL}
			}											// must replace ls with its path though (/bin/ls)
			leftof[numI - 2] = NULL;								// look at above example command
			//printf("the first elemenet of leftof is %s\n",leftof[0]);				// strcpy( leftof[0], func(leftof) ) func must return path of ls as a char *

			if(     getcmdloc(leftof[0]) == false           )                                       // command doesnt exist
                                printf("%s: command not found\n", bucket[0]);
			else
			{

				if (strcmp(bucket[numI - 2],">") == 0 )
					redir(leftof, bucket[numI -1], 'w');

				if(strcmp(bucket[numI - 2],"<") == 0 )
					redir(leftof, bucket[numI -1], 'r');
			}
		}

		else												// now try to exec the command
		{
			if( 	getcmdloc(bucket[0]) == false		)					// command doesnt exist
				printf("%s: command not found\n", bucket[0]);

			else											// command exist and should be run with exec
			{
				 //printf("the new bucket[0] is %s\n", bucket[0] );
				 //printf("the bucket[1] is %s\n", bucket[1] );
				 //printf("the new bucket[2] is %s\n", bucket[2] );
				 //printf("the new bucket[3] is %s\n", bucket[3] );

				char ** leftof = malloc( (numI + 1) * sizeof(char*) );
                        	//char * cmd1[4] = {"/bin/ls", "-l", "-a", NULL};                                        example test command to ake sure this section w$
				int i;
                        	for(i = 0; i < numI ; i++)
                        	{
                                	leftof[i] = bucket[i];
                                	//printf("the string copied is: %s\n", leftof[i]);                                // left of now has {"ls","-l","-a", NULL}
                        	}
                                            								                 // must replace ls with its path though (/bin/ls)
                        	leftof[numI] = NULL;
				myexec(leftof, false);
			}
		}									// end executing command else

	     }										// end if not echo cmd

	  } 										//until "exit" is read in


	free(bucket);    								//free dynamic memory
	printf("Exiting...\n\t Session Time:  %lds\n", timedif);
	return 0;
}									// end main
									//reallocates instruction array to hold another token		//returns new pointer to instruction array
char** addToken(char** instr, char* tok, int numTokens)
{
	int i;
	char** new_arr;
	new_arr = (char**)malloc((numTokens+1) * sizeof(char*));	//copy values into new array
	for (i = 0; i < numTokens; i++)
	{
		new_arr[i] = (char *)malloc((strlen(instr[i])+1) * sizeof(char));
		strcpy(new_arr[i], instr[i]);
	}
	//add new token
	new_arr[numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
	strcpy(new_arr[numTokens], tok);
	if (numTokens > 0)
		free(instr);
	return new_arr;
}
void printTokens(char** instr, int numTokens)
{
	int i;
	printf("number of tokens (numI) is: %d\n", numTokens);
	printf("Tokens:\n");
	for (i = 0; i < numTokens; i++)
		printf("#%s# token number = %d\n", instr[i], i);
}
