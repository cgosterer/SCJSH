#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>


void prompt() {
  printf("%s@%s :: %s ->", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
}

/* Utility function takes in a string and replaces any section of the form $NAME with Environment variable's VALUE*/
/* Returns 0 on a success, returns 1 if a variable is undefined*/
int readEnv(char * str, int size) {
  char *temp1, *temp2;
  char name[size];
  char * value;
  int i, j, k, tempsize;
  tempsize = size;
  temp1 = (char*) calloc(tempsize, sizeof(char));
  /* Loop through each character, looking for $'s to indicate environment variables*/
  for(i = j = 0; i < size & str[i] != '\0'; i++, j++)
    {
      if(str[i] != '$')
	temp1[j] = str[i];
      else
	{
	  temp1[j] = '\0';
	  k = 0;
	  i++;
	  while(isalnum(str[i]) || str[i] == '_')
	    {
	      name[k] = str[i];
	      i++;
	      k++;
	    }
	  i--;
	  name[k] = '\0';
	  value = getenv(name);
	  if(value == 0)
	    {
	      free(temp1);
	      return 1;
	    }
	  temp2 = (char *) calloc(tempsize, sizeof(char));
	  strcpy(temp2, temp1);
	  free(temp1);
	  tempsize += strlen(value);
	  temp1 = (char *) calloc(tempsize, sizeof(char));
	  strcpy(temp1, temp2);
	  free(temp2);
	  strcat(temp1, value);
	  j += strlen(value) - 1;
	}	
    }
  strcpy(str, temp1);
  free(temp1);
  return 0;
}
