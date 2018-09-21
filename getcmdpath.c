#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool getcmdloc(char * cmd)
{
	FILE * canopen;					// test to see if we can open the file
	char * envpath = getenv("PATH");		// string with the env path
	char * envtok = (char *) malloc (strlen(envpath));
	strcpy(envtok, envpath);
	const char delim[2] = ":";			// delimiter for tokenizing
	char * testprefix;				// will hold the finalpath of the command
	char * prefix;					// will hold each path token from the $PATH variablke

	int docounter = 1;
	do
	{
		if (docounter == 1)
		{
			prefix = strtok(envtok, delim );
			if (prefix == NULL)
				break;
			testprefix = (char *) malloc (	(strlen(prefix) + 1 + strlen(cmd) + 1) * sizeof(char)	);
			strcpy(testprefix, prefix);
			strcat(testprefix,"/");
                        strcat(testprefix,cmd);

			if(canopen = fopen(testprefix,"r"))
			{
				fclose(canopen);
				strcpy(cmd,testprefix);
				free(testprefix);
				free(envtok);
				return true;
			}
		}

		if (docounter > 1)
		{
			prefix = strtok( NULL, delim );
			if (prefix == NULL)
				break;
			testprefix = (char *) malloc ( (strlen(prefix) + 1 + strlen(cmd) + 1)  * sizeof(char));
                        strcpy(testprefix, prefix);
			strcat(testprefix,"/");
			strcat(testprefix,cmd);

			if(canopen = fopen(testprefix,"r"))
                        {
				strcpy(cmd,testprefix);
                                fclose(canopen);
				free(testprefix);
				free(envtok);
				return true;
                        }
		}
		docounter++;
		free(testprefix);

	}while(prefix != NULL);				// end while

	free(envtok);
	return false;
}							// end getcmdpath

