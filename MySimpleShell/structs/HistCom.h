// Brad Howard
// Lab 7 - USH

// History Sturct file

#ifndef HISTCOM_H_
#define HISTCOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 **	This is the histCom struct for linkedlist for command history
 **/
 
struct histCom
{
	int histNum;
	char * command;
};

typedef struct histCom HistCommand;

void cleanTypeHistCommand(void * ptr);

void * buildTypeHistCommand(char *com, int n);

int compareHist(const void * p1, const void * p2);

void printTypeHistCommand(void * passedIn);

#endif
