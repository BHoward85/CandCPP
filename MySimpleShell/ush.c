#include "core.h"

int main()
{
	// working data set	
	int argc, pipeCount, dirTest, histCount = 0, histFileCount = 0;	
	char **argv = NULL, s[MAX], r[MAX];
	int preCount = 0, postCount = 0, histNum = 1, index, dout = 0, din = 0;
	char ** prePipe = NULL, ** postPipe = NULL;
	char * dir = NULL, * prevDir = NULL, * temp = NULL;
	LinkedList * history = NULL, * aliases = NULL;

	// start up
	aliases = linkedList(100, 100);

	readrc(&histCount, &histFileCount, aliases);

	history = linkedList(histCount, histFileCount);
	readHist(history, &histNum);

	//printf("HC: %d HFC: %d\n", histCount, histFileCount);

	printf("%s :: cmd?: ", getcwd(s, MAX));
	fgets(s, MAX, stdin);
	strip(s);

	getalias(aliases, s);
	
	if(strcmp(s, "!!") != 0 && strstr(s , "!") == NULL && strstr(s, "|") == NULL)
		addLast(history, buildHistNodeByString(s, histNum++, buildTypeHistCommand));
	//printf("History count is: %d\n", history->size);

	while(strcmp(s, "exit") != 0)
	{
		pipeCount = containsPipe(s);
		if(pipeCount > 0)
		{
			if(pipeCount == 1) // dose command sub for hist (!!, !n)
			{
				prePipe = parsePrePipe(s, &preCount, history, aliases);
				postPipe = parsePostPipe(s, &postCount, history, aliases);
				pipeIt(prePipe, postPipe);

				char newS[MAX] = "\0";
				for(index = 0; index < preCount; index++)
				{
					strcat(newS, prePipe[index]);
					strcat(newS, " ");
				}
				strcat(newS, "| ");
				for(index = 0; index < postCount; index++)
				{
					strcat(newS, postPipe[index]);
					if(index < postCount - 1)					
						strcat(newS, " ");
				}
				addLast(history, buildHistNodeByString(newS, histNum++, buildTypeHistCommand));
				//printf("new str: %s\n", newS);

				clean(preCount, prePipe);
				clean(postCount, postPipe);
			}
			else
				printf("USH: currently do not support more than one pipe.\n");
		}// end if pipeCount	  
		else
		{
			argc = makeargs(s, &argv);

			for(index = 0; index < argc; index++)
			{
				if(strcmp(argv[index], "<") == 0)
					din = index;
				else if(strcmp(argv[index], ">") == 0)
					dout = index;
			}

			if(din > 0 && dout > 0)
			{
				//printf("din @%d dout @%d\n", din, dout);
				redirectinout(din, dout, argv);
			}
			else if(din > 0)
			{
				//printf("din @%d\n", din);
				redirectin(din, argv);
			}
			else if(dout > 0)
			{
				//printf("dout @%d\n", dout);
				redirectout(dout, argv);
			}			
			else if(strcmp(argv[0], "cd") == 0 && argc > 1)
			{
				dirTest = getdir(argv[1], &dir, prevDir);

				temp = getcwd(s, MAX);
				strip(temp);

				if(prevDir == NULL)
					prevDir = (char *)calloc(strlen(temp) + 1, sizeof(char));
				else
				{
					free(prevDir);
					prevDir = (char *)calloc(strlen(temp) + 1, sizeof(char));
				}
				strcpy(prevDir, temp);

				//printf("pdir :: %s\n", prevDir);
				
				if(dirTest == 0)
				{
					dirTest = chdir(dir);
					
					if(dirTest == 0)
						printf("Move to %s\n", getcwd(s, MAX));
					else if(dirTest == -1)
						printf("No such directory\n");
					else
						printf("There was a error with the command input, please check your command.");
				}
				else
				{
					printf("There was a error with the command input, please check your command.");
				}
				
				free(dir);
				dir = NULL;
			}
			else if(strcmp(argv[0], "history") == 0)
			{
				//@TODO: history - Done
				if(history->size > histCount)
					printSubList(history, printTypeHistCommand);
				else
					printList(history, printTypeHistCommand);
				//printf("Hist\n");
			}
			else if(strcmp(argv[0], "alias") == 0)
			{
				//@TODO: alias
				if(argc == 1)
					printList(aliases, printTypeAlias);
				else
				{					
					//printf("Command %s\n name %s\n actcmd %s\n", argv[0], argv[1], argv[2]);
					addLast(aliases, buildAliasNodeByString(argv[1], argv[2], buildTypeAlias));
				}
					
			}
			else if(strcmp(argv[0], "unalias") == 0)
			{
				//@TODO: unalias
				//printf("Unalias\n");

				if(argc > 1)
				{
					removeItem(aliases, buildAliasNodeByString(argv[1], " ", buildTypeAlias), cleanTypeAlias, compareAlias);
				}
				else
				{
					printf("Please pick an Alias to unalias\n");
					printList(aliases, printTypeAlias);
				}
			}
			else if(strcmp(argv[0], "!!") == 0)
			{
				//@TODO: get last command and forkIt - Done
				//printf("Run last\n");
				HistCommand *temp;// = (HistCommand *)history->tail->prev->data;
				getlasthist(history, &temp);
				//printf("Command at tail: %s\n", temp->command);
				strcpy(s, temp->command);

				addLast(history, buildHistNodeByString(s, histNum++, buildTypeHistCommand));
				
				clean(argc, argv);
				argv = NULL;

				continue;
			}
			else if(strstr(argv[0], "!") != NULL)
			{
				//@TODO: get a command that is at nth place in hist - done
				//printf("Run num\n");

				HistCommand *temp;
				char *st;
				int nth;
				char *t;

 				t = strtok_r(argv[0], "!", &st);
				//printf("%s\n", t);		
				
				nth = atoi(t);

				getnthhist(history, &temp, nth);
				strcpy(s, temp->command);
				//printf("%s\n", s);
				
				addLast(history, buildHistNodeByString(s, histNum++, buildTypeHistCommand));
				clean(argc, argv);
				argv = NULL;

				continue;
			}
			else
			{
				if(argc != -1)
					forkIt(argv);
			}
			
			din = 0;
			dout = 0;
			clean(argc, argv);
			argv = NULL;
		}

		printf("%s :: cmd?: ", getcwd(s, MAX));
		fgets(s, MAX, stdin);
		strip(s);

		getalias(aliases, s);
	
		if(strcmp(s, "!!") != 0 && strstr(s , "!") == NULL && strstr(s, "|") == NULL)
			addLast(history, buildHistNodeByString(s, histNum++, buildTypeHistCommand));
		//printf("History count is: %d\n", history->size);

	}// end while

	// shut down processes	
	writeHist(history);

	dirclean(dir, prevDir);

	clearList(history, cleanTypeHistCommand);
	free(history);
	history = NULL;

	clearList(aliases, cleanTypeAlias);
	free(aliases);
	aliases = NULL;
	
	return 0;

}// end main

// core functions - I didn't have time to refactor these in to a core.c file

void dirclean(char * dir, char * prevDir)
{
	free(prevDir);
	prevDir = NULL;
	free(dir);
	prevDir = NULL;
}

void readrc(int * histCount, int * histFileCount, LinkedList * aliases)
{
	FILE * fin;
	char * temp = NULL;
	char * aliasName = NULL;
	char * aliasCmd = NULL;
	char set[MAX];
	char * stopper;
	int n = 0;
	int mode = 0;

	fin = openRCFile(".ushrc", &mode);

	//printf("FIN = %d\n", fin != NULL);

	while(n < 2)
	{
		fgets(set, MAX, fin);
		strip(set);
		//printf("ushrc file 155: %s\n", set);
		temp = strtok_r(set, "=", &stopper);
		//printf("ushrc file 157: %s\n", temp);

		if(strcmp(temp, "HISTCOUNT") == 0)
		{
			temp = strtok_r(NULL, "=", &stopper);
			//printf("ushrc file 162: %s\n", temp);
			*histCount = atoi(temp);
		}
		else if(strcmp(temp, "HISTFILECOUNT") == 0)
		{
			temp = strtok_r(NULL, "=", &stopper);
			//printf("ushrc file 168: %s\n", temp);
			*histFileCount = atoi(temp);
		}
		n++;
	}
	
	fgets(set, MAX, fin);
	while(TRUE)
	{
		fgets(set, MAX, fin);
		if(feof(fin)) break;
		strip(set);
		
		if(strstr(set, "alias") != 0 || strstr(set, "Alias") != 0)
		{
			temp = strtok_r(set, " ", &stopper);
			//printf("first: %s\n", temp); // get alias command
			aliasName = strtok_r(NULL, "=", &stopper);
			strip(aliasName);
			//printf("mid: %s\n", aliasName); // get alias tag
			aliasCmd = strtok_r(NULL, "\'", &stopper);
			strip(aliasCmd);
			//printf("last: %s\n", aliasCmd); // get act command
			addLast(aliases, buildAliasNodeByString(aliasName, aliasCmd, buildTypeAlias));
			//printf("%s - %s\n", aliasName, aliasCmd);
		}		
	}

	fclose(fin);
}

void writeHist(LinkedList * history)
{
	FILE * fout;
	int mode = 0;

	fout = writeHISTFile(".ush_history", &mode);

	writeSubList(fout, history);

	fclose(fout);
}

void readHist(LinkedList * history, int * histNum)
{
	FILE * fin;
	char set[MAX] = " ";
	int mode = 0;

	fin = readHISTFile(".ush_history", &mode);

	if(mode == 1)
	{
		while(TRUE)
		{
			fgets(set, MAX, fin);
			if(feof(fin)) break;
			strip(set);
			addLast(history, buildHistNodeByString(set, (*histNum)++, buildTypeHistCommand));
		}

		fclose(fin);
	}
}

void getnthhist(LinkedList * history, HistCommand ** data, int histNum)
{
	Node *curr = history->head->next;
	HistCommand *temp;

	while(curr->next != NULL)
	{
		if(curr->data != NULL)
		{
			temp = (HistCommand *)curr->data;

			if(temp->histNum == histNum)
			{
				*data = temp;
				return;
			}
		}
		curr = curr->next;
	}
}

void getlasthist(LinkedList * history, HistCommand ** data)
{
	*data = (HistCommand *)history->tail->prev->data;
}

void getalias(LinkedList * aliases, char * str)
{
	Node *curr = aliases->head->next;
	Alias data;
	//printf("in getalias\n");
	while(curr != NULL)
	{
		Alias data = *((Alias *)curr->data);

		//printf("%s\n", data.aliasName);
		//printf("%s\n", data.command);

		if(strcmp(str, data.aliasName) == 0)
		{
			strcpy(str, data.command);
			//printf("Command: %s\n", str);
			return;
		}
		curr = curr->next;
	}		
}

char * replacealias(LinkedList * aliases, char * str)
{
	char *actcommand;
	Node *curr = aliases->head->next;
	Alias data;
	//printf("in replace alias\n");
	while(curr != NULL)
	{
		Alias data = *((Alias *)curr->data);

		//printf("%s\n", data.aliasName);
		//printf("%s\n", data.command);
		//printf("%s\n", str);

		//printf("%d\n", strcmp(str, data.aliasName));
		if(strcmp(str, data.aliasName) == 0)
		{
			actcommand = (char *)calloc(strlen(data.command) + 1, sizeof(char));
			strcpy(actcommand, data.command);
			//printf("Command: %s\n", actcommand);
			return actcommand;
		}
		curr = curr->next;
	}
	return str;
}

void redirectout(int dout, char **argv)
{
	pid_t pid;
	int stat;
	
	int out, num;
	char **args;

	//printf("after din: %s\nafter dout: %s\ncommand %s\n", argv[din + 1], argv[dout + 1], argv[0]);

	args = (char **)calloc(dout + 1, sizeof(char *));

	for(num = 0; num < dout; num++)
	{
		args[num] = (char *)calloc(strlen(argv[num]) + 1, sizeof(char));
		strcpy(args[num], argv[num]);
		//printf("%s\n", args[num]);
	}
	
	pid = fork();

	if(pid != 0)
	{
		waitpid(pid, &stat, 0);
		
		for(num = 0; num < dout; num++)
		{
			free(args[num]);
		}
		free(args);
	}
	else
	{
		out = open(argv[dout + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

		dup2(out, 1);

		close(out);

		execvp(args[0], args);
		exit(-1);
	}
}

void redirectin(int din, char **argv)
{
	pid_t pid;
	int stat;
	
	int in, num;
	char **args;

	//printf("after din: %s\nafter dout: %s\ncommand %s\n", argv[din + 1], argv[dout + 1], argv[0]);

	args = (char **)calloc(din + 1, sizeof(char *));

	for(num = 0; num < din; num++)
	{
		args[num] = (char *)calloc(strlen(argv[num]) + 1, sizeof(char));
		strcpy(args[num], argv[num]);
		//printf("%s\n", args[num]);
	}
	
	pid = fork();

	if(pid != 0)
	{
		waitpid(pid, &stat, 0);
		
		for(num = 0; num < din; num++)
		{
			free(args[num]);
		}
		free(args);
	}
	else
	{
		if(access(argv[din + 1], F_OK) != -1)
			in = open(argv[din + 1], O_RDONLY);
		else
		{
			printf("file not found error");
			return;
		}

		dup2(in, 0);
		
		close(in);
		
		execvp(args[0], args);
		exit(-1);
	}
}

void redirectinout(int din, int dout, char **argv)
{
	pid_t pid;
	int stat;
	
	int in, out, num;
	char **args;

	//printf("after din: %s\nafter dout: %s\ncommand %s\n", argv[din + 1], argv[dout + 1], argv[0]);

	args = (char **)calloc(din + 1, sizeof(char *));

	for(num = 0; num < din; num++)
	{
		args[num] = (char *)calloc(strlen(argv[num]) + 1, sizeof(char));
		strcpy(args[num], argv[num]);
		//printf("%s\n", args[num]);
	}
	
	pid = fork();

	if(pid != 0)
	{
		waitpid(pid, &stat, 0);
		
		for(num = 0; num < din; num++)
		{
			free(args[num]);
		}
		free(args);
	}
	else
	{
		if(access(argv[din + 1], F_OK) != -1)
			in = open(argv[din + 1], O_RDONLY);
		else
		{
			printf("file not found error");
			return;
		}

		out = open(argv[dout + 1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

		dup2(in, 0);
		dup2(out, 1);

		close(in);
		close(out);

		execvp(args[0], args);
		exit(-1);
	}
}
