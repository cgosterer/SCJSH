#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool getcmdloc(char * cmd)
{
	printf("the passed command is %s\n", cmd);
	FILE * canopen;					// test to see if we can open the file
	char * envpath = getenv("PATH");		// string with the env path
	char * envtok = (char *) malloc (strlen(envpath));
	strcpy(envtok, envpath);
	printf("path: %s\n", envtok);
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
			testprefix = (char *) malloc (strlen(prefix) + strlen(cmd) + 1);
			strcpy(testprefix, prefix);
			strcat(testprefix,"/");
                        strcat(testprefix,cmd);
			printf("%s\n", testprefix);

			if(canopen = fopen(testprefix,"r"))
			{
				fclose(canopen);
				strcpy(cmd,testprefix);
				//return testprefix;
				return true;
			}
		}

		if (docounter > 1)
		{
			prefix = strtok( NULL, delim );
			if (prefix == NULL)
				break;
			testprefix = (char *) malloc (strlen(prefix) + strlen(cmd) + 1);
                        strcpy(testprefix, prefix);
			strcat(testprefix,"/");
			strcat(testprefix,cmd);
			printf("%s\n", testprefix);

			if(canopen = fopen(testprefix,"r"))
                        {
				strcpy(cmd,testprefix);
                                //return testprefix;
                                fclose(canopen);
				return true;
                        }
		}
		docounter++;

	}while(prefix != NULL);				// end while

	return false;
}							// end getcmdpath

/*
int main()
{
	printf( "The location of the env command is:##%s##\n" , getcmdloc("envas"));
	return 0;
}
*/
