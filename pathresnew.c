#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
char * absolutePath(char * pathwayOptions, char * command){

//char * commandPath;
char *r = strdup;
// check for errors
char * temp, *r, *tok;

r = temp = strdup("/home/majors/molina/.bin:/home/majors/molina/.scripts:/usr/local/bin:/opt/sfw/bin:/usr/sfw/bin:/bin:/usr/bin:/usr/ccs/bin:/usr/ucb:");
assert(temp != NULL);

while ((tok = strsep(&temp, ":")) != NULL) {
										//put string into an array of strings or 
	if(fileCheck(tok, command) == 1)	//check if command is in tok + /command, if it is return string. 
		return tok;						//if it isnt go to next string seperated by colon and check
	
	printf("%s\n", tok);  // testing
}

free(r);
}
bool fileCheck(char * pathway, char * command){
	pathway = strcat("/", pathway);
	pathway = strcat(command, pathway);
	if (file_exists(pathway) == 1){
	return 1;}
	else 
		return 0;
	
}// takes in string and adds "/command" to end. then the string is tested with file_exists returns 0 or 1


bool file_exists(const char * filename)
{
    FILE * file;
	file = fopen(filename, "r");
	if (file){		//file exists and can be open
		fclose(file);// close file when you're done
        return 1;}
	
	else{
		return 0; //file doesn't exists or cannot be opened (es. you don't have access permission 
		}
}







