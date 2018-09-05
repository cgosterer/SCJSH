#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


char** addToken(char ** instr, char * tok, int numTokens);
void printTokens(char** instr, int numTokens);

int main()
{
	struct timeval start;
	struct timeval end;
	long int timedif;						// time difference between start and end of shell
	int timedummy1 = gettimeofday(&start, NULL);			// timedummy1 only used to make sure return val doesnt mean error
	//printf("%ld seconds\n", start.tv_sec);


	int has_exit = 0;
	char token[256];        // holds instruction token
	char ** bucket;         // array that holds all instruction tokens
	char temp[256];		// used to split instruction tokens containing special characters

while (has_exit == 0)
{

	printf("Please enter an instruction:");
	int numI = 0;	                // number of tokens in an instruction
	do {                            // loop reads character sequences  separated by whitespace
		scanf( "%s", token);

		if ( strcmp(token, "exit") == 0)
		{
		        int timedummy2  = gettimeofday(&end, NULL);		// timedummy2 only used to see if error is returned
			timedif = end.tv_sec - start.tv_sec;
        		//printf("%ld seconds\n", end.tv_sec);

			has_exit = 1;
			//break;
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

	}while('\n' != getchar());     //until end of line is reached, end do while loop

	printTokens(bucket, numI);	// print the tokens

} 					//until "exit" is read in


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






























