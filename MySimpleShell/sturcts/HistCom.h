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
	char * command;
	char ** tokcom;
};

typedef struct histCom HistCommand;

 void cleanTypeHistCommand(void * ptr);
 
 void * buildTypeHistCommand(char *com, char **tokCom);

#endif