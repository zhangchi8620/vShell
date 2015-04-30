#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./Types/types.h"
#include "./Filesystem/filesystem.h"
#include "./Interpreter/interpreter.h"

char cmdLine[100000];

int main()
{
	time_t u, v;
	(void) time(&u);
	(void) time(&v);
//	fs_mkfs();
	initCache();
	buildRoot();
	buildDirectoryTree(dir_root);
	while (1)
	{
		printf("%d %d\n", u, v);
		if ((int)v-(int)u > 10)
		{
			u = v;
			refreshCache();
		}
		(void) time(&v);
		char rt[1000];
		memset(rt, 0, sizeof(rt));
		getFullPath(dir_pwd, rt);
		printf("%s %%", rt);
		memset(cmdLine, 0, sizeof(cmdLine));
		while (strlen(cmdLine) == 0)
			gets(cmdLine);
		if (strcmp(cmdLine, "exit") == 0 || strcmp(cmdLine, "quit") == 0 \
			|| strcmp(cmdLine, "e") == 0 || strcmp(cmdLine, "q") == 0) break;
		parseCommand(cmdLine);
	}
	return 0;
}
