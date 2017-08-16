// Brad Howard
// Lab5 - Forkit

// Fork it C file

#include "process.h"

void forkIt(char ** argv)
{
	pid_t pid = fork();
	int stat;

	if(pid != 0)
	{
		waitpid(pid, &stat, 0);
	}
	else
	{
		execvp(argv[0], argv);
		exit(1);
	}
}
