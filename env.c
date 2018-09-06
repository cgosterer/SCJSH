#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>

void prompt() {
  printf("%s@%s :: %s ->", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
}

int readEnv(char * str, int size) {
  char temp[size];
  char name[size];
  char * value;
  int i, j, k;
  for(i = 0; i < size; i++)
    {
      temp[i] = '\0';
    }
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
  return 0; //0 indicates successful run
}
