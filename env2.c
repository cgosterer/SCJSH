#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>

char cwd[100];

void prompt() {
  getcwd(cwd, sizeof cwd);
  printf("%s@%s :: %s ->", getenv("USER"), getenv("MACHINE"), cwd );
}

/* Utility function takes in a string and replaces any section of the form $NAME with Environment variable's VALUE*/
/* Returns 0 on a success, returns 1 if the variable is undefined*/
int readEnv(char * str, int size) {
  char temp[size];
  char name[size];
  char * value;
  int i, j, k;
  /* Initializes temp string to null characters to avoid potential errors*/
  for(i = 0; i < size; i++)
    {
      temp[i] = '\0';
    }
  /* Loop through each character, looking for $'s to indicate environment variables*/
  for(i = j = 0; i < size & str[i] != '\0'; i++, j++)
    {
      if(str[i] != '$')
	temp[j] = str[i];
      else
	{
	  temp[j] = '\0';
	  k = 0;
	  i++;
	  while(isalnum(str[i]))
	    {
	      name[k] = str[i];
	      i++;
	      k++;
	    }
	  i--;
	  name[k] = '\0';
	  value = getenv(name);
	  if(value == 0)
	    return 1;
	  strcat(temp, value);
	  j += strlen(value) - 1;
	}	
    }
  strcpy(str, temp);
  return 0;
}
