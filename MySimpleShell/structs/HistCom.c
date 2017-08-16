// Brad Howard
// Lab7 - Shell

// HistCom C file

#include "HistCom.h"

void cleanTypeHistCommand(void * ptr)
{
	if(ptr == NULL)
	{
		perror("Null ptr error : Clean Hist : ");
		exit(-99);
	}
	
	int index;
	
	HistCommand *hc = (HistCommand *)ptr;

	//printf("hist number: %d\n", hc->histNum);
	free(hc->command);
	
	free(hc);
	hc = NULL;
}
 
void * buildTypeHistCommand(char *com, int num)
{
	if(com == NULL)
	{
		perror("Null ptr error : build Hist : ");
		exit(-99);
	}
	
	HistCommand *newHist = (HistCommand *)malloc(sizeof(HistCommand));
	
	newHist->command = (char *)calloc(strlen(com) + 1, sizeof(char));
	strcpy(newHist->command, com);
	
	newHist->histNum = num;

	return (void *)newHist;
}

int compareHist(const void * p1, const void * p2)
{
	
	if(p1 == NULL || p2 == NULL)
	{
		perror("Null pointer error : compare hist : ");
		exit(-99);
	}
	
	HistCommand left = *((HistCommand *)p1);
	HistCommand right = *((HistCommand *)p2);
	
	return strcmp(left.command, right.command);
}

void printTypeHistCommand(void * passedIn)
{
	if(passedIn == NULL)
	{
		perror("Null pointer error : print Hist : ");
		exit(-99);
	}
	
	HistCommand *hc = (HistCommand *)passedIn;
	
	printf("%d %s\n", hc->histNum, hc->command);
}
