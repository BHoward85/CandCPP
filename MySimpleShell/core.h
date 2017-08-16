// Brad Howard
// Lab 7 - USH

// Main support C file

#ifndef CORE_H_
#define CORE_H_

#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./utils/fileUtil.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./linkedlist/listUtils.h"
#include "./linkedlist/linkedList.h"
#include "./linkedlist/requiredIncludes.h"
#include <fcntl.h>

#define TRUE 1

void readrc(int * histCount, int * histFileCount, LinkedList * aliases);

void dirclean(char * dir, char * prevDir);

void writeHist(LinkedList * history);

void readHist(LinkedList * history, int * histNum);

void getnthhist(LinkedList * history, HistCommand ** data, int histNum);

void getlasthist(LinkedList * history, HistCommand ** data);

void getalias(LinkedList * aliases, char * str);

char * replacealias(LinkedList * aliases, char * str);

void redirectout(int dout, char **argv);

void redirectin(int din, char **argv);

void redirectinout(int din, int dout, char **argv);

#endif


