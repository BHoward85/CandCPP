#include "myUtils.h"

void strip(char *array)
{
	if(array == NULL)
	{
		perror("array is null");
		exit(-99);
	}// end if

	int len = strlen(array), x = 0;
   
	while(array[x] != '\0' && x < len)
	{
	  if(array[x] == '\r')
		 array[x] = '\0';

	  else if(array[x] == '\n')
		 array[x] = '\0';

	  x++;
	}// end while
   
}// end strip

int getdir(char *s, char **dir, const char *prevDir)
{
	char *copy = NULL;
	char *stopper;
	char *temp;
	
	if(s == NULL)
		return -2;

	if(strcmp(s, "~") == 0)
	{
		temp = getenv("HOME");
		copy = (char *)calloc(strlen(temp) + 1, sizeof(char));
		strcpy(copy, temp);
	}
	else if(strcmp(s, "-") == 0 && prevDir != NULL)
	{
		copy = (char *)calloc(strlen(prevDir) + 1, sizeof(char));
		strcpy(copy, prevDir);
	}
	else
	{
		copy = (char *)calloc(strlen(s) + 1, sizeof(char));
		strcpy(copy, s);
	}
	
	*dir = (char *)calloc(strlen(copy) + 1, sizeof(char));
	
	strcpy(*dir, copy);
	
	free(copy);
	
	if(*dir != NULL)
		return 0;
	else // there was no dir in the cd command
		return -1;
}
