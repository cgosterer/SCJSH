#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "./env.c"
#include "exec.c"
#include "getcmdpath.c"
#include "exec_and_io.c"

char** addToken(char ** instr, char * tok, int numTokens);
void printTokens(char** instr, int numTokens);
int executeTokens(char** instr, int numTokens);

int main()
{
	//queue = malloc(100 * sizeof(process));

        struct timeval start;
	struct timeval end;
	long int timedif;						// time difference between start and end of shell
	int timedummy1 = gettimeofday(&start, NULL);			// timedummy1 only used to make sure return val doesnt mean error
	//printf("%ld seconds\n", start.tv_sec);

	const int INST_SIZE = 256;
	int has_exit = 0;
	char token[INST_SIZE];          // holds instruction token
	char ** bucket;         	// array that holds all instruction tokens
	char temp[INST_SIZE];		// used to split instruction tokens containing special characters

	while (has_exit == 0)
	  {

	    prompt();
	    int numI = 0;	                // number of tokens in an instruction
	    do {                            // loop reads character sequences  separated by whitespace
	      scanf( "%s", token);
	      readEnv(token, INST_SIZE);
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

	    }while('\n' != getchar());     //until end of line is reached, end do while loop

	    has_exit = executeTokens(bucket, numI);	// executes the instruction based on the tokens, sets has_exit if needed

	  } 					//until "exit" is read in

	int timedummy2  = gettimeofday(&end, NULL);// timedummy2 only used to see if error is returned
	timedif = end.tv_sec - start.tv_sec;

	free(bucket);    			//free dynamic memory
	printf("Exiting...\n\t Session Time:  %lds\n", timedif);
	return 0;
}

//reallocates instruction array to hold another token,
//returns new pointer to instruction array
char** addToken(char** instr, char* tok, int numTokens)
{
	int i;
	char** new_arr;
	new_arr = (char**)malloc((numTokens+1) * sizeof(char*));

	//copy values into new array
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
	printf("Tokens:\n");
	for (i = 0; i < numTokens; i++)
		printf("#%s#\n", instr[i]);
}

int executeTokens(char** instr, int numTokens)
{
  int i, j, k;
  char **cmnd;
  char *buff;
  for(i = 0; i < numTokens; i++)
    {
      j = i;
      while(j < numTokens && strcmp(instr[j], "|") != 0)
	j++;
      cmnd = (char**) malloc((j-i+1) * sizeof(char*));
      for(k = 0; k < j - i; k++)
	{
	  cmnd[k] = instr[i + k];
	}
      cmnd[k] = NULL;
      if(strcmp(cmnd[0], "exit") == 0)
	return 1;
      else if(strcmp(cmnd[0], "echo") == 0)
	{
	  for(k = 1; cmnd[k] != NULL; k++)
	    {
	      printf("%s ", cmnd[k]);
	    }
	  printf("\n");
	}
      else if(strcmp(cmnd[0], "cd") == 0)
	{
	  chdir(cmnd[1]);
	  k = pathconf(".", _PC_PATH_MAX) * sizeof(char);
	  buff = (char *) malloc(k);
	  setenv("PWD", getcwd(buff, k), 1);
	  free(buff);
	}

	else if (strcmp(cmnd[0], "io") == 0)
	{
		char ** minusio = malloc(numTokens * sizeof(char*));
		int i;
		for ( i = 1; i < numTokens; i++)					// getting rid of io so we can run the actual command for now
			minusio[i-1] = cmnd[i];
		minusio[numTokens - 1] = NULL;

		if( getcmdloc(minusio[0]) == false )                                       // if command doesnt exist show error message
                                printf("%s: command not found\n", cmnd[0]);
		else
			myexecio(minusio, true);
	}

      else										// execute the command
	{
		if( getcmdloc(cmnd[0]) == false )					// if command doesnt exist show error message
				printf("%s: command not found\n", cmnd[0]);
		else
			myexec(cmnd);
	}
      free(cmnd);
      i = j;
    }
  return 0;
}
