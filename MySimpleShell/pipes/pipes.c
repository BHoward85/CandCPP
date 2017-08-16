// Brad Howard
// Lab 6 - Pipe It

// Pipes C file

#include "pipes.h"
#include "../core.h"

int containsPipe(char * s)
{
	int count = 0;
	int index = 0;
	char tempChar = ' ';

	while(s[index++] != '\0')
	{
		if(s[index] == '|')
			count++;
	}

	//printf("Number of Pipe in the string: %d\n", count);
	return count;
}

char ** parsePrePipe(char * s, int * preCount, LinkedList * history, LinkedList * aliases)
{
	//printf("in pre pipe\n");
	HistCommand *hdata;
	Alias *adata;
	char *subCopy = NULL;
	char newTemp[MAX] = "A";
	char pattern[MAX] = "!!";
	char *numc = NULL;
	int nth = 0;

	char **prePipe = NULL;
	char *temp = NULL;
	char *copy = NULL;
	char *stopper = NULL;

	copy = (char *)calloc(strlen(s) + 1, sizeof(char));
	strcpy(copy, s);

	//printf("memalloced\n");

	temp = strtok_r(copy, "|", &stopper);

	//printf("temp = %s\n", temp);

	if(temp[1] == pattern[1])
	{
		//printf("Pre pipe: !! : %s : %s\n", temp, newTemp);
		getlasthist(history, &hdata);
		strcpy(newTemp, hdata->command);

		*preCount = makeargs(newTemp, &prePipe);
	}
	else if(temp[1] != pattern[1] && strstr(temp, "!") != NULL)
	{
		//printf("Pre pipe: ! : %s : %s\n", temp, newTemp);
		subCopy = (char *)calloc(strlen(temp) + 1, sizeof(char));
		strcpy(subCopy, temp);

		//printf("Pre pipe memalloc: ! : %s : %s\n", subCopy, newTemp);
		numc = strtok_r(subCopy, "!", &stopper);

		nth = atoi(numc);
		
		getnthhist(history, &hdata, nth);
		
		strcpy(newTemp, hdata->command);

		*preCount = makeargs(newTemp, &prePipe);

		free(subCopy);
		subCopy = NULL;
	}
	else	
	{
		int i;
		char *tempStr = (char *)calloc(strlen(temp), sizeof(char));

		for(i = 0; i < strlen(temp) - 1; i++)
		{
			tempStr[i] = temp[i];
		}			

		//printf("temp: %s\ntempstr: %s\n", temp, tempStr);
		//char *snip = strtok_r(temp, " ", &stopper);
		//printf("snip: %s\n%s\n", temp, tempStr);
		subCopy = replacealias(aliases, tempStr);	
		*preCount = makeargs(subCopy, &prePipe);
		if(strcmp(subCopy, tempStr) != 0)
			free(subCopy);
		free(tempStr);
	}
	free(copy);
	copy = NULL;

	//printf("the PrePipe is:\n");
	//printargs(*preCount, prePipe);
	//printf("with a count of %d\n", *preCount);
	return prePipe;
}

char ** parsePostPipe(char * s, int * postCount, LinkedList * history, LinkedList * aliases)
{
	HistCommand *hdata;
	Alias *adata;
	char *subCopy = NULL;
	char newTemp[MAX] = "A";
	char pattern[MAX] = "!!";
	char *numc = NULL;
	int nth = 0;

	char **postPipe = NULL;
	char *temp = NULL;
	char *copy = NULL;
	char *stopper = NULL;

	copy = (char *)calloc(strlen(s) + 1, sizeof(char));
	strcpy(copy, s);

	temp = strtok_r(copy, "|", &stopper);

	if(temp != NULL)
	{
		temp = strtok_r(NULL, "|", &stopper);

		if(temp[1] == pattern[1])
		{
			//printf("Pre pipe: !! : %s : %s\n", temp, newTemp);
			getlasthist(history, &hdata);
			strcpy(newTemp, hdata->command);

			*postCount = makeargs(newTemp, &postPipe);
		}
		else if(temp[1] != pattern[1] && strstr(temp, "!") != NULL)
		{
			//printf("Pre pipe: ! : %s : %s\n", temp, newTemp);
			subCopy = (char *)calloc(strlen(temp) + 1, sizeof(char));
			strcpy(subCopy, temp);

			//printf("Pre pipe memalloc: ! : %s : %s\n", subCopy, newTemp);
			numc = strtok_r(subCopy, "!", &stopper);

			nth = atoi(numc);
		
			getnthhist(history, &hdata, nth);
		
			strcpy(newTemp, hdata->command);

			*postCount = makeargs(newTemp, &postPipe);

			free(subCopy);
			subCopy = NULL;
		}
		else	
		{	
			int i;
			char tempChar;
			for(i = 1; i < strlen(temp); i++)
			{
				tempChar = temp[i];
				temp[i - 1] = tempChar;
				if(i == strlen(temp) - 1)
					temp[i] = ' ';
			}

			char *tempStr = (char *)calloc(strlen(temp), sizeof(char));

			for(i = 0; i < strlen(temp) - 1; i++)
			{
				tempStr[i] = temp[i];
			}			

			//printf("temp: %s\ntempstr: %s\n", temp, tempStr);
			//char *snip = strtok_r(temp, " ", &stopper);
			//printf("snip: %s\n%s\n", temp, tempStr);
			subCopy = replacealias(aliases, tempStr);	
			*postCount = makeargs(subCopy, &postPipe);
			if(strcmp(subCopy, tempStr) != 0)
				free(subCopy);
			free(tempStr);
		}
	}

	free(copy);
	copy = NULL;

	//printf("the PostPipe is:\n");
	//printargs(*postCount, postPipe);
	//printf("with a count of %d\n", *postCount);
	return postPipe;
}

void pipeIt(char ** prePipe, char ** postPipe)
{
	/**
	** @note: run under basic and cap the out then fix it
	**/

	pid_t pid;
	int fd[2];
	int res;
	int status;

	pid = fork();

	if(pid != 0)
	{
		waitpid(pid, &status, 0);
	}
	else
	{

		res = pipe(fd);

		if(res < 0)
		{
			printf("Pipe Failure\n");
			exit(-1);
		}

		pid = fork();

		if(pid != 0)
		{
			waitpid(pid, &status, 0);
			close(fd[1]);
			close(0);
			dup(fd[0]);
			close(fd[0]);
			execvp(postPipe[0], postPipe);
			exit(-1);
		}	
		else
		{	
			close(fd[0]);
			close(1);
			dup(fd[1]);
			close(fd[1]);
			execvp(prePipe[0], prePipe);
			exit(-1);
		}
	}
}
