// Brad Howard
// Lab 7 - USH

// Alias struct file

#ifndef ALIAS_H_
#define ALIAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 **	This is the Alias struct for linkedlist for command alias
 **/
 
 struct alias
 {
	char * aliasCom;
	char ** tokCom;
 };
 
 typedef struct alias Alias;
 
 void cleanTypeAlias(void * ptr);
 
 void * buildTypeAlias(char *nameAlias, char *command);
 
 #endif