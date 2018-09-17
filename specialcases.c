char* shrinkDirectory(char* path); // for ..
char* resolveSpecialChars(char* str); //resolves . .. ~
char* replaceSpecialChars(char* dest, size_t start, size_t end, const char* source); //replaces characters bewtween start and end. source is inserted
char* removeDynamicChar(char* line, size_t start, size_t end); // removes chars from dynamic arrays
char* strPush(char* dest, char source); // pushes a char onto a dynam array


char* strPush(char* dest, char ch)
{
	size_t setLength = strlen(dest) + 2;
	char* dynamicStr = (char*)calloc(setLength, sizeof(char));
	strcpy(dynamicStr, dest);
	dynamicStr[setLength-2] = ch;
	dynamicStr[setLength-1] = '\0';
	free(dest);
	return dynamicStr;
}

char* replaceSpecialChars(char* dest, size_t start, size_t end, const char* source)
{
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

	if (start == 0)
		itrLast = end + 1;
	else
		itrLast = end + 1;

	for (newPathItr = 0; newPathItr < finalLength; newPathItr++)
	{
		if (newPathItr < start)
			newPath[newPathItr] = dest[itrDest++];
		else if ((newPathItr >= start) && (newPathItr <= start+lengthOfSource-1))
			newPath[newPathItr] = source[itrSource++];
		else
			newPath[newPathItr] = dest[itrLast++];
	}
	free(dest);
	return newPath;
}


char* resolveSpecialChars(char* str)
{
	char* absPath = str;

	if ((strlen(absPath) == 1) && (strcmp(absPath, "/") == 0))
		return absPath;

	// check for ~
	if (absPath[0] == '~')
	{
		absPath = replaceSpecialChars(absPath, 0, 0, getenv("HOME"));
	}

	
	if (absPath[0] == '.' && absPath[1] != '.')// conditional for .
	{
		if (strcmp(getenv("PWD"), "/") == 0) //in root, delete both . and /
			absPath = replaceSpecialChars(absPath, 0, 1, getenv("PWD"));
		
		else   // else, only need to delete .
			absPath = replaceSpecialChars(absPath, 0, 0, getenv("PWD"));
	}

	if (absPath[0] == '.' && absPath[1] == '.')  // conditional for ..
	{
		char* pwd = getenv("PWD");
		char* newPwd = (char*)calloc(strlen(pwd)+1,sizeof(char));
		strcpy(newPwd, pwd);
		newPwd = shrinkDirectory(newPwd);
		// attempt to hardcode fix changing to / from /home
		//if (strcmp(newPwd,"")==0)
		//{
		//	absPath = replaceSpecialChars(absPath, 0, strlen(absPath)-1, "/");
		//	free(newPwd);
		//	return absPath;
		//}
		absPath = replaceSpecialChars(absPath, 0, 1, newPwd);
		free(newPwd);
	}

	size_t strTracker = 1;
	size_t tempSlash = 0;
	size_t tempSlashTwo = 0;
	while (absPath[strTracker] != '\0')  // loop to look for . and .. anywhere in string
	{
		if (absPath[strTracker] == '.' && absPath[strTracker-1] == '.')
		{
			if (strTracker > 2)  //conditional for root
			{
				absPath = removeDynamicChar(absPath, tempSlashTwo, strTracker); 
				strTracker = 0;// begin with 0 to recognize '/' locations
				tempSlash = 0;
				tempSlashTwo = 0;
				if (strcmp(absPath,"") == 0)// empty string, down to root
				{
					absPath = strPush(absPath, '/');
					return absPath;
				}
			}
			else if (strlen(absPath) > 3)
			{
				absPath = removeDynamicChar(absPath, strTracker-2, strTracker);
				strTracker = strTracker - 3;	// will be -1, ++strTracker at end of loop will make 0
			}
			else
			{
				absPath = removeDynamicChar(absPath, strTracker-1, strTracker);
				strTracker = strTracker - 2;
			}
		}
		// check strlen to make sure no invalid memory read
		else if ((strlen(absPath) >= 2) && absPath[strTracker-1] == '.' && absPath[strTracker] == '/')
		{
			absPath = removeDynamicChar(absPath, strTracker-1, strTracker);
			strTracker = strTracker - 2;
		}
		else if (absPath[strTracker] == '/')
		{
			tempSlashTwo = tempSlash;
			tempSlash = strTracker;
		}
		++strTracker;
	}

	return absPath;
}

char* removeDynamicChar(char* line, size_t end, size_t start)  // removes characters from a dynamic array
{

	size_t number_characters = end - start + 1;
	int strLength = strlen(line)+1;
	int dynamicLength = strLength - (int)number_characters;
	char* dynamicLine = (char*)calloc(dynamicLength, sizeof(char));
	dynamicLine[dynamicLength - 1] = '\0';//'\0' at the end
	size_t tempItr1 = 0;
	size_t tempItr2 = 0;
	char tempChar = line[tempItr1];
	while (tempChar != '\0')
	{
		if (!((tempItr1 >= start) && (tempItr1 <= end)))
		{
			dynamicLine[tempItr2++] = tempChar;
		}
		tempChar = line[++tempItr1];
	}
	free(line);
	line = NULL;
	return dynamicLine;
}

char* shrinkDirectory(char* path)   // removes a directory from a path
{
	int temp = strlen(path) - 1;
	int arrayIndex = temp;
	char deletedChar = path[temp];

	if (deletedChar == '/' && temp == 0)// root directory, do nothing
		return path;
	
	
	if (deletedChar == '/')   // '/' at end of path
	{
		deletedChar = path[--temp];
	}
	

	while (deletedChar != '/')   // search for next '/'
	{
		deletedChar = path[--temp];
	}

	return removeDynamicChar(path, temp, arrayIndex);
}

