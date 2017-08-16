#ifndef PIPES_H
#define PIPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../linkedlist/listUtils.h"
#include "../linkedlist/linkedList.h"
#include "../tokenize/makeArgs.h"

int containsPipe(char *s);
char ** parsePrePipe(char *s, int * preCount, LinkedList * history, LinkedList * aliases);
char ** parsePostPipe(char *s, int * postCount, LinkedList * history, LinkedList * aliases);
void pipeIt(char ** prePipe, char ** postPipe);


#endif 
