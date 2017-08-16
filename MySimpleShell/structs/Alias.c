// Brad Howard
// Lab7 - Shell

// Alias C file

#include "Alias.h"

void cleanTypeAlias(void * ptr)
{
	if(ptr == NULL)
	{
		perror("Null ptr error : Clean Hist : ");
		exit(-99);
	}
	
	Alias *ac = (Alias *)ptr;

	free(ac->aliasName);
	free(ac->command);
	
	free(ac);
	ac = NULL;
}

void * buildTypeAlias(char *name, char *cmd)
{
	if(cmd == NULL || name == NULL)
	{
		perror("Null ptr error : build Hist : ");
		exit(-99);
	}
	
	Alias *newAlias = (Alias *)malloc(sizeof(Alias));
	
	newAlias->aliasName = (char *)calloc(strlen(name) + 1, sizeof(char));
	strcpy(newAlias->aliasName, name);
	//printf("%s - %s\n", newAlias->aliasName, name);
	
	newAlias->command = (char *)calloc(strlen(cmd) + 1, sizeof(char));
	strcpy(newAlias->command, cmd);
	//printf("%s - %s\n", newAlias->command, cmd);

	return(void *)newAlias;
}

int compareAlias(const void * p1, const void * p2)
{
	
	if(p1 == NULL || p2 == NULL)
	{
		perror("Null pointer error : compare hist : ");
		exit(-99);
	}
	
	Alias left = *((Alias *)p1);
	Alias right = *((Alias *)p2);
	
	return strcmp(left.aliasName, right.aliasName);
}

void printTypeAlias(void * passedIn)
{
	if(passedIn == NULL)
	{
		perror("Null pointer error : print Hist : ");
		exit(-99);
	}
	
	Alias *ac = (Alias *)passedIn;
	
	if(ac->aliasName != NULL && ac->command != NULL)
		printf("alias %s=\'%s\'\n", ac->aliasName, ac->command);
}

