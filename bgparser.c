#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "./env.c"
#include "getcmdpath.c"
#include "finalexec.c"
#include "specialcases.c"

char** addToken(char ** instr, char * tok, int numTokens);
void printTokens(char** instr, int numTokens);
int executeTokens(char** instr, int numTokens);
int recursivePipe(char** instr, int numTokens, int hasio, int isbg, int input);
void emptyBucket(char** instr, int numTokens);

int main()
{
	queue = malloc(100 * sizeof(process));
        struct timeval start;
	struct timeval end;
	long int timedif;						// time difference between start and end of shell
	int timedummy1 = gettimeofday(&start, NULL);			// timedummy1 only used to make sure return val doesnt mean error

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
	    emptyBucket(bucket, numI);
	  } 					//until "exit" is read in

	int timedummy2  = gettimeofday(&end, NULL);// timedummy2 only used to see if error is returned
	timedif = end.tv_sec - start.tv_sec;

	//free(queue);
	freeq();
	printf("Exiting...\n\t Session Time:  %lds\n", timedif);
	return 0;
}

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
	  emptyBucket(instr, numTokens);
	return new_arr;
}

void emptyBucket(char** instr, int numTokens)
{
  int i;
  for(i = 0; i < numTokens; i++)
    {
      free(instr[i]);
    }
  free(instr);
}

void printTokens(char** instr, int numTokens)
{
	int i;
	printf("Tokens:\n");
	for (i = 0; i < numTokens; i++)
		printf("#%s#\n", instr[i]);
}

void changeDir(char* target)
{
  char* buff;
  int k;
  if(target[0] == '~')
    {
      target = resolveSpecialChars(target);
    }
  chdir(target);
  k = pathconf(".", _PC_PATH_MAX) * sizeof(char);
  buff = (char *) malloc(k);
  setenv("PWD", getcwd(buff, k), 1);
  free(buff);
}

int executeTokens(char** instruct, int numTokens)
{
  int i, j, k;
  char **instr = instruct;
  char **cmnd = 0;
  char *buff = 0;
  int isbg = 0;
  int hasRedir = 0;
  int hasio = 0;
  i = 0;
  //While loops and if statement remove any &'s at start and back
  //Also checks for io
  while(strcmp(instr[i], "&") == 0)
    i++;
  j = numTokens;
  while(strcmp(instr[j - 1], "&") == 0)
    {
      j--;
      isbg = 1;
    }
  if(strcmp(instr[i], "io") == 0)
    {
      hasio = 1;
      i++;
    }
  if(i != 0 || j != numTokens)
    {
      cmnd = (char**) malloc((j-i) * sizeof(char*));
      for(k = 0; k < j - i; k++)
	{
	  cmnd[k] = instr[k + i];
	}
      instr = cmnd;
      cmnd = 0;
      numTokens = j - i;
    }
  if(strcmp(instr[0], "exit") == 0)
    {
      if(instr != instruct)
	free(instr);
      return 1;
    }
  //printTokens(instr, numTokens);
  //Checks whether any &'s remain, if so print error message
  //Also checks for redirection
  for(i = 0; i < numTokens; i++)
    {
      if(strcmp(instr[i], "&") == 0)
	{
	  printf("Error: Unexpected \'&\' token\n");
	  if(instr != instruct)
	    free(instr);
	  return 0;
	}
      else if(strcmp(instr[i], "<") == 0)
	hasRedir = 1;
      else if(strcmp(instr[i], ">") == 0)
	hasRedir = 1;
    }
  if(hasRedir == 1)
    {
      if( strcmp(instr[0], "echo") != 0 && strcmp(instr[0], "cd") != 0 && getcmdloc(instr[0]) == false )
	{
	  printf("Error: Command %s not found", instr[0]);
	  if(instr != instruct)
	    free(instr);
	  return 0;
	}

      //printf("Redirection detected\n");
      k = j = 0;
      //We're now using hasRedir to index cmnd
      cmnd = (char**) malloc(numTokens * sizeof(char*));
      for(i = 0, hasRedir = 0; i < numTokens; i++)
	{
	  if(strcmp(instr[i], ">") == 0)
	    {
	      //printf("Opening %s\n", instr[i + 1]);
	      j = open( instr[i + 1], O_RDWR | O_TRUNC | O_CREAT , S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR );
	      if(j == -1)
		{
		  free(cmnd);
		  printf("Error: Could Not Open or Create Output Redirection File\n");
		  if(instr != instruct)
		    free(instr);
		  return 0;
		}
	      i++;
	    }
	  else if(strcmp(instr[i], "<") == 0)
	    {
	      k = open(instr[i + 1], O_RDONLY);
	      if(k == -1)
		{
		  free(cmnd);
		  printf("Error: Input Redirection File Not Found\n");
		  if(instr != instruct)
		    free(instr);
		  return 0;
		}
	      i++;
	    }
	  else
	    {
	      cmnd[hasRedir] = instr[i];
	      hasRedir++;
	    }
	}
      cmnd[hasRedir] = NULL;
      if(strcmp(cmnd[0], "cd") == 0)
	{
	  changeDir(cmnd[1]);
	}
      else
	myexec(cmnd, hasio, isbg, k, j);
      free(cmnd);
      if(instr != instruct)
	free(instr);
      return 0;
    }
  //printf("No redirection detected\n");
  return recursivePipe(instr, numTokens, hasio, isbg, 0);
}

int recursivePipe(char** instr, int numTokens, int hasio, int isbg, int input)
{
  int i, j, k, piper[2];
  char ** cmnd;
  char * buff;
  piper[0] = piper[1] = 0;
  for(j = 1; j < numTokens; j++)
    {
      if(strcmp(instr[j], "|") == 0)
	{
	  k = pipe(piper);
	  if(k == -1)
	    {
	      printf("Unknown error while creating pipe, please try again\n");
	      return 0;
	    }
	  //printf("Input = %d, Output = %d\n", piper[0], piper[1]);
	  break;
	}
    }
  cmnd = (char**) malloc((j+1) * sizeof(char*));
  for(k = 0; k < j; k++)
    {
      cmnd[k] = instr[k];
    }
  cmnd[k] = NULL;
  if(strcmp(cmnd[0], "exit") == 0)
    {
      free(cmnd);
      return 1;
    }
  //Every command except the final command is run as if it's in background mode.  This should simulate them requesting information from the previous process.
  //The final command is run in the mode we actually want, so the shell itself will decide whether or not to wait for it
  if(j == numTokens)
    {
      if(strcmp(cmnd[0], "cd") == 0)
	changeDir(cmnd[1]);
      else
	{
	  if( strcmp(instr[0], "echo") != 0 && strcmp(instr[0], "cd") != 0 && getcmdloc(instr[0]) == false )
	    {
	      printf("Error: Command %s not found\n", instr[0]);
	      free(cmnd);
	      return 0;
	    }
	  //printf("Finally Running %s\n", cmnd[0]);
	  //printf("hasio = %d, isbg = %d, input = %d\n", hasio, isbg, input);
	  myexec(cmnd, hasio, isbg, input, 0);
	}
      //printf("%s has finished\n", cmnd[0]);
      free(cmnd);
      return 0;
    }
  else
    {
      if(strcmp(cmnd[0], "cd") == 0)
	changeDir(cmnd[1]);
      else
	{
	  if( strcmp(instr[0], "echo") != 0 && strcmp(instr[0], "cd") != 0 && getcmdloc(instr[0]) == false )
	    {
	      printf("Error: Command %s not found", instr[0]);
	      if(input != 0)
		close(input);
	      close(piper[0]);
	      close(piper[1]);
	      return 0;
	    }
	  //printf("Running %s\n", cmnd[0]);
	  myexec(cmnd, hasio, isbg, input, piper[1]);
	  close(piper[1]);
	  //printf("%s has finished\n", cmnd[0]);
	}
      if(strcmp(instr[k+1], "io") == 0)
	{
	  i = recursivePipe(instr + k + 2, numTokens - k - 2, 1, isbg, piper[0]);
	}
      else
	{
	  i = recursivePipe(instr + k + 1, numTokens - k - 1, 0, isbg, piper[0]);
	}
      close(piper[0]);
      free(cmnd);
      return i;
    }
}

