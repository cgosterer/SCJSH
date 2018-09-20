#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char* shrinkDirectory(char* path); // for ..
char* resolveSpecialChars(char* str); //resolves . .. ~
char* replaceSpecialChars(char* dest, size_t start, size_t end, const char* source); //replaces characters bewtween start and end. source is inserted
char* removeDynamicChar(char* line, size_t start, size_t end); // removes chars from dynamic arrays
char* strPush(char* dest, char source); // pushes a char onto a dynam array
//for testing
int main(){
	//printf("PATH : %s\n", getenv("PATH"));
   	//printf("HOME : %s\n", getenv("HOME"));
   	//printf("PWD : %s\n", getenv("PWD"));
	
	char* pathWorkingDir = getenv("PWD");
	char* testHome = getenv("HOME");

	if(strcmp("/home/majors/molina/OSproj1", pathWorkingDir) != 0){
		printf("Test pwd is wrong\n");
	} else {
		printf("Test pwd is right\n");
	}
	

	printf("Starting text X\n");
	if(strcmp("gonna fail", resolveSpecialChars("./")) != 0){
		printf("Test X is wrong\n");
	} else {
		printf("Test X is right\n");
	}

	printf("Starting test pwd real\n");
	if(strcmp(pathWorkingDir, resolveSpecialChars(".")) != 0){
		printf("Test text pwd is wrong\n");
		printf("current Dir: %s\n", pathWorkingDir);
		printf("test ./ : %s\n", resolveSpecialChars("./"));
	} else {
		printf("Test text pwd is right\n");
	}

	printf("Starting test home dir real\n");
	if(strcmp(testHome, resolveSpecialChars("~")) != 0){
		printf("Test home dir is wrong\n");
		printf("Home dir: %s\n", testHome);
		printf("test ~/ : %s\n", resolveSpecialChars("~"));
	} else {
		printf("Test text pwd is right\n");
	}
	/*
	printf("Starting test dummy dir real\n");
	char* testDummyDir = strcat(pathWorkingDir, "/dummy_dir");
	if(strcmp(testDummyDir, resolveSpecialChars("dummy_dir/")) != 0){
		printf("Test home dir is wrong\n");
		printf("dir: %s\n", testDummyDir);
		printf("test ~/ : %s\n", resolveSpecialChars("dummy_dir/"));  fixed this test case
	} else {
		printf("Test text pwd is right\n");
	}
	*/
	printf("Starting .. real\n");
        char* goUp = "/home/majors/molina";
        if(strcmp(goUp, resolveSpecialChars("..")) != 0){
                printf("Test home dir is wrong\n");
                printf("dir: %s\n", goUp);
                printf("test ../ : %s\n", resolveSpecialChars(".."));
        } else {
                printf("Test text pwd is right\n");
                printf("test ../ : %s\n", resolveSpecialChars(".."));
        }



	return 0;
}
//end of testing
char* strPush(char* dest, char ch){
	size_t setLength = strlen(dest) + 2;
	char* dynamicStr = (char*)calloc(setLength, sizeof(char));
	strcpy(dynamicStr, dest);
	dynamicStr[setLength-2] = ch;
	dynamicStr[setLength-1] = '\0';
	free(dest);
	return dynamicStr;
}

char* replaceSpecialChars(char* dest, size_t start, size_t end, const char* source){
	//printf("got to replace\n");
	size_t tempIndex = end - start + 1;
	size_t lengthOfDest = strlen(dest);
	size_t lengthOfSource = strlen(source);
	size_t finalLength = lengthOfDest - tempIndex + lengthOfSource;

	char* newPath = (char*)calloc(finalLength + 1, sizeof(char));
	newPath[finalLength - 1] = '\0'; 

	int newPathItr = 0;
	int itrDest = 0;
	int itrSource = 0;
	int itrLast = 0;
	//printf("mid of replace\n");
	if (start == 0)
		itrLast = end + 1;
	else
		itrLast = end + 1;

	for (newPathItr = 0; newPathItr < finalLength; newPathItr++){
		if (newPathItr < start)
			newPath[newPathItr] = dest[itrDest++];
		else if ((newPathItr >= start) && (newPathItr <= start+lengthOfSource-1))
			newPath[newPathItr] = source[itrSource++];
		else
			newPath[newPathItr] = dest[itrLast++];
	}
	//printf("end of replace\n");
	//free(dest);
	//printf("end of replace after free\n");
	
	return newPath;
}


char* resolveSpecialChars(char* str){
	//printf("got to resolve");
	char* absPath = str;

	if ((strlen(absPath) == 1) && (strcmp(absPath, "/") == 0))
		return absPath;

	// check for ~
	if (absPath[0] == '~'){
		char* h = getenv("HOME");
		absPath = replaceSpecialChars(absPath, 0, 0, h);
	}
	else if(absPath[0] != '.' || (absPath[0] != '.' && absPath[1] != '.') ){
		// printf("this is str : %s\n", str);
                char* prefix = getenv("PWD");
               // printf("this is prefix : %s\n", prefix)  
                return prefix;
	}

	//printf("got to 2 of resolve\n");
	if (absPath[0] == '.' && absPath[1] != '.'){// conditional for .
		//printf("got to inside of conditional for .\n");
		//printf("%s", getenv("PWD"));
		
		char* pwd = getenv("PWD");
		if (strcmp(pwd, "/") == 0){ //in root, delete both . and /
			//printf("got to deletion of . and /\n");
			absPath = replaceSpecialChars(absPath, 0, 1, pwd);
		}
		else{   // else, only need to delete .
			//printf("got to else for delete .\n");
			absPath = replaceSpecialChars(absPath, 0, 0, pwd);
		}
	}
	//printf("got to 2.5 of resolve\n");
	if (absPath[0] == '.' && absPath[1] == '.'){  // conditional for ..
		char* pwd = getenv("PWD");
		char* newPwd = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(newPwd, pwd);
		newPwd = shrinkDirectory(newPwd);
		absPath = replaceSpecialChars(absPath, 0, 1, newPwd);
		free(newPwd);
	}
	//printf("got to 3 of resolve\n");
	size_t strTracker = 1;
	size_t tempSlash = 0;
	size_t tempSlashTwo = 0;
	while (absPath[strTracker] != '\0'){  // loop to look for . and .. anywhere in string

		if (absPath[strTracker] == '.' && absPath[strTracker-1] == '.'){
			if (strTracker > 2){  //conditional for root
				absPath = removeDynamicChar(absPath, tempSlashTwo, strTracker); 
				strTracker = 0;// begin with 0 to recognize '/' locations
				tempSlash = 0;
				tempSlashTwo = 0;
				if (strcmp(absPath,"") == 0){// empty string, down to root
					absPath = strPush(absPath, '/');
					return absPath;
				}
			}
			else if (strlen(absPath) > 3){
				absPath = removeDynamicChar(absPath, strTracker-2, strTracker);
				strTracker = strTracker - 3;	// will be -1, ++strTracker at end of loop will make 0
			}
			else{
				absPath = removeDynamicChar(absPath, strTracker-1, strTracker);
				strTracker = strTracker - 2;
			}
		}
		// check strlen to make sure no invalid memory read
		else if ((strlen(absPath) >= 2) && absPath[strTracker-1] == '.' && absPath[strTracker] == '/'){
			absPath = removeDynamicChar(absPath, strTracker-1, strTracker);
			strTracker = strTracker - 2;
		}
		else if (absPath[strTracker] == '/'){
			tempSlashTwo = tempSlash;
			tempSlash = strTracker;
		}
		++strTracker;
	}
	//printf("got to end of resolve\n");
	return absPath;
}

char* removeDynamicChar(char* line, size_t start, size_t end){  // removes characters from a dynamic array


	size_t number_characters = end - start + 1;
	int strLength = strlen(line)+1;
	int dynamicLength = strLength - (int)number_characters;
	char* dynamicLine = (char*)calloc(dynamicLength, sizeof(char));
	dynamicLine[dynamicLength - 1] = '\0';//'\0' at the end
	size_t tempItr1 = 0;
	size_t tempItr2 = 0;
	char tempChar = line[tempItr1];
	while (tempChar != '\0'){
		if (!((tempItr1 >= start) && (tempItr1 <= end))){
			dynamicLine[tempItr2++] = tempChar;
		}
		tempChar = line[++tempItr1];
	}
	free(line);
	//line = NULL;
	return dynamicLine;
}

char* shrinkDirectory(char* path){   // removes a directory from a path
  printf("The path passed in is %s\n", path);
	int temp = strlen(path) - 1;
	int arrayIndex = temp;
	char deletedChar = path[temp];

	if (deletedChar == '/' && temp == 0)// root directory, do nothing
		return path;
	
	
	if (deletedChar == '/'){   // '/' at end of path
		deletedChar = path[--temp];
	}
	

	while (deletedChar != '/'){   // search for next '/'
	
		deletedChar = path[--temp];
	}

	char* newDynPath = removeDynamicChar(path, temp, arrayIndex);
	printf("The returned string is %s\n", newDynPath);
        return newDynPath;
}


