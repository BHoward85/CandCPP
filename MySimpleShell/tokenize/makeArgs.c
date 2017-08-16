#include "makeArgs.h"

void clean(int argc, char **argv)
{
	int index;
	
	for(index = 0; index < argc; index++)
	{
		free(argv[index]);
	}
	free(argv);
}// end clean

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}// end printargs

int makeargs(char *s, char *** argv)
{
	int n = 0;
	int total = 0;
	int index;
	char tempChar = '\0';
	char *copy;
	char *temp;
	char *stopper;

	char *alias;
	char *aliasName;
	char *aliasCmd;
	
	copy = (char *)calloc(strlen(s) + 1, sizeof(char));
	strcpy(copy, s);
	
	temp = strtok_r(s, " ", &stopper);

	if(strcmp(temp, "alias") == 0)
	{
		if(strstr(copy, "=") != NULL)
		{
			n = 1;
			alias = strtok_r(copy, " ", &stopper);	
			//printf("first: %s\n", alias); // get alias command
			aliasName = strtok_r(NULL, "=", &stopper);
			strip(aliasName);

			if(aliasName != NULL)
			{		
				n++;
				//printf("mid: %s\n", aliasName); // get alias tag
				aliasCmd = strtok_r(NULL, "\'", &stopper);
				strip(aliasCmd);

				if(aliasCmd != NULL)
					n++;

				//printf("last: %s\n", aliasCmd); // get act command
			}

			*argv = (char **)calloc(n + 1, sizeof(char *));
		
			(*argv)[0] = (char *)calloc(strlen(alias) + 1, sizeof(char));
			strcpy((*argv)[0], alias);
		
			if(n > 2)
			{
				(*argv)[1] = (char *)calloc(strlen(aliasName) + 1, sizeof(char));
				strcpy((*argv)[1], aliasName);
			
				(*argv)[2] = (char *)calloc(strlen(aliasCmd) + 1, sizeof(char));
				strcpy((*argv)[2], aliasCmd);
			}
			total = n;
		}
		else
		{
			//printf("%s\n", temp);			
			*argv = (char **)calloc(1, sizeof(char *));
			(*argv)[0] = (char *)calloc(strlen(temp) + 1, sizeof(char));
			strcpy((*argv)[0], temp);
			total = 1;
		}
	}
	else
	{
		while(temp != NULL)
		{
			total++;
			temp = strtok_r(NULL, " ", &stopper);
		}
	
		*argv = (char **)calloc(total + 1, sizeof(char *));
	
		temp = strtok_r(copy, " ", &stopper);

		for(index = 0; index < total; index++)
		{
			(*argv)[index] = (char *)calloc(strlen(temp) + 1, sizeof(char));
			strcpy((*argv)[index], temp);
			temp = strtok_r(NULL, " ", &stopper);
		}
	}	

	free(copy);
	return total;
}// end makeArgs
