#include "interpreter.h"
#include "../Types/types.h"
#include "../Filesystem/filesystem.h"

DIRECTORY *dir_root;
DIRECTORY *dir_pwd;
char fbuffer[10000];
extern FILE *out;
extern bool toFile;
bool onClose;

int parseCommand (char *cmd)
{
	if (cmd[0] == '#') return 0;
	int argc = 0;
	char argv[10][10000];
	memset (argv, 0, sizeof(argv));
	int nn = 0;
	for (int i = 0; i < strlen(cmd); i++)
	{
		if (cmd[i] == '"') break;
		nn++;
	}

	memset(argv, 0, sizeof(argv));

	int r = 0, start = 0;
	while (cmd[r] == ' ' && r < nn) r++;
	for (; r < nn; )
	{
		start = r;
		while (cmd[r] != ' ' && r < nn) r++;
		strncpy(argv[argc++], cmd+start, r-start);
		while (cmd[r] == ' ' && r < nn) r++;
	}
	if (nn < strlen(cmd))
		strncpy(argv[argc++], cmd+nn+1, strlen(cmd)-nn-2);

	if (argc == 1)
	{
		if (strcmp(argv[0], "ls") == 0)
		{
			if (dir_pwd->nchild == 65535)
			{
				if (toFile)
					fprintf(out, "./\t../\t\n");
				else
					printf("./\t../\t\n");
			}
			else
			{
				if (toFile)
					fprintf(out, "./\t../\t\n");
				else
					printf("./\t../\t");
				for (int i = 0; i < dir_pwd->nchild; i++)
				{
					if (toFile)
						fprintf(out, "%s\t", dir_pwd->child[i]->dirName);
					else
						printf("%s\t", dir_pwd->child[i]->dirName);
				}
				if (toFile)
					fprintf(out, "\n");
				else
					printf("\n");
			}
			return 0;
		}
		else if (strcmp(argv[0], "mkfs") == 0)
		{
			fs_mkfs();
			return 0;
		}
		else if (strcmp(argv[0], "pwd") == 0)
		{
			char rt[1000];
			char t[1000];
			memset(rt, 0, sizeof(rt));
			memset(t, 0, sizeof(t));
			getFullPath(dir_pwd, t);
			strncpy(rt, t+1, strlen(t)-2);
			if (toFile)
				fprintf(out, "%s\n", rt);
			else
				puts(rt);
			return 0;
		}
		else if (strcmp(argv[0], "tree") == 0)
		{
			fs_tree(dir_root, 0);
			return 0;
		}
		else
		{
			FILE *fp = fopen(argv[0], "r");
			if (fp == NULL)
			{
				if (toFile) fprintf(out, "Wrong command or file does not exist.\n");
				else printf("Wrong command or file does not exist.\n");
				return -1;
			}
			char line[10000];
			memset(line, 0, sizeof(line));
			while (fgets(line, 10000, fp))
			{
				if (line[0] == '\n') continue;
				line[strlen(line)-1] = '\0';
				parseCommand(line);
				memset(line, 0, sizeof(line));
			}
			fclose(fp);
			return 0;
		}
	}
	else if (argc == 2)
	{
		if (strcmp(argv[0], "cd") == 0)
		{
			if (strcmp(argv[1], "..") == 0) dir_pwd = dir_pwd->parent;
			else if (strcmp(argv[1], ".") == 0) dir_pwd = dir_pwd;
			else
			{
				DIRECTORY *x = getDirectory(argv[1]);
				if (x == dir_pwd)
				{
					if (toFile)
						fprintf(out, "Shell Error: cd\nNo such file exist.");
					else
						puts("Shell Error: cd\nNo such file exist.");
					return -1;
				}
				else if (x->inode == 1)
				{
					if (toFile)
						fprintf(out, "Not a directory.");
					else
						puts("Not a directory.");
					return -1;
				}
				else 
				{
					dir_pwd = x;
					return 0;
				}
			}
			return 0;
		}
		if (strcmp(argv[0], "mkdir") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p != dir_pwd)
			{
				if (toFile) fprintf(out, "Directory Exist: %s\n", argv[1]);
				else printf("Directory Exist: %s\n", argv[1]);
				return -1;
			}
			if (fs_mkdir(argv[1]))
			{
				if (toFile)
					fprintf(out, "Shell Error: fs_mkdir()\n");
				else
					puts("Shell Error: fs_mkdir()");
				return -1;
			}
			return 0;
		}
		if (strcmp(argv[0], "rmdir") == 0)
		{			
			if (getDirectory(argv[1]) == dir_pwd)
			{
				if (toFile)
					fprintf(out, "Directory does not exist.\n");
				else
					puts("Directory does not exist.");
				return -1;
			}
			if (fs_rmdir(argv[1]))
			{
				if (toFile)
					fprintf(out, "Shell Error: fs_rmdir()");
				else
					puts("Shell Error: fs_rmdir()");
				return -1;
			}
			return 0;
		}
		if (strcmp(argv[0], "close") == 0)
		{
			long long x = 0;
			for (int i = 0; i < strlen(argv[1]); i++)
				x = x*10 + (argv[1][i] - '0');
			int kk = checkCache(x);
			if (kk > 0) fs_write(x, cache->frame[kk], strlen(cache->frame[kk]));
			fs_close(x);
			return 0;
		}
		if (strcmp(argv[0], "unlink") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p == dir_pwd)
			{
				if (toFile)
					fprintf(out, "No such file to unlink.\n");
				else
					puts("No such file to unlink.");
				return -1;
			}
			fs_unlink(argv[1]);
			return 0;
		}
		if (strcmp(argv[0], "stat") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p == dir_pwd)
			{
				if (toFile)
					fprintf(out, "No such file to stat.\n");
				else
					puts("No such file to stat.");
				return -1;
			}
			fs_stat(argv[1]);
			return 0;
		}
		if (strcmp(argv[0], "cat") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p == dir_pwd)
			{
				if (toFile)
					fprintf(out, "No such file to cat.\n");
				else
					puts("No such file to cat.");
				return -1;
			}
			char buf[10000];
			memset(buf, 0, sizeof(buf));
			int kk = checkCache((p->childAddr[0]-1)*512);
			if (kk > 0) strncpy(buf, cache->frame[kk], strlen(cache->frame[kk]));
			else fs_read((p->childAddr[0]-1)*512, buf, 10000);
			for (int i = 0; i < strlen(buf); i++)
				if (buf[i] == '~') buf[i] = '\0';
			if (toFile)
				fprintf(out, "%s\n", buf);
			else
				puts(buf);
			return 0;
		}

	}
	else if (argc == 3)
	{
		if (strcmp(argv[0], "open") == 0)
		{
			int m = 0;
			if (strcmp(argv[2], "r") == 0 || strcmp(argv[2], "1") == 0) m = 1;
			if (strcmp(argv[2], "w") == 0 || strcmp(argv[2], "2") == 0) m = 2;
			if (strcmp(argv[2], "rw") == 0 || strcmp(argv[2], "3") == 0) m = 3;
			fs_open(argv[1], m);
			if (FD < 0)
			{
				if (toFile)
					fprintf(out, "Open File Failed.\n");
				else
					puts ("Open File Failed.");
				return -1;
			}
			else
			{
				if (toFile)
					fprintf (out, "SUCCESS: fd = %d\n", FD);
				else
					printf ("SUCCESS: fd = %d\n", FD);
				return 0;
			}
		}
		if (strcmp (argv[0], "read") == 0)
		{
			long long t = 0;
			int cnt = 0;
			char buf[10000];
			memset(buf, 0, sizeof(buf));
			for (int i = 0; i < strlen(argv[1]); i++)
				t = t*10 + argv[1][i]-'0';
			for (int i = 0; i < strlen(argv[2]); i++)
				cnt = cnt*10 + argv[2][i]-'0';
			int kk = checkCache(t);
			if (kk > 0)
			{
				strncpy(buf, cache->frame[kk], strlen(cache->frame[kk]));
				if (toFile)
					fprintf(out, "%s\n", buf);
				else
					puts (buf);
			}
			else if (!fs_read(t, buf, cnt))
			{
				if (toFile)
					fprintf(out, "%s\n", buf);
				else
					puts (buf);
			}
			else 
			{
				if (toFile)
					fprintf(out, "Read Failed.\n");
				else
					puts ("Read Failed.");
				return -1;
			}
			return 0;
		}


		if (strcmp (argv[0], "write") == 0)
		{
			long long t = 0;
			for (int i = 0; i < strlen(argv[1]); i++)
				t = t*10 + argv[1][i]-'0';

			int kk = checkCache(t);
			if (kk > 0)
			{
				memset(cache->frame[kk], 0, sizeof(cache->frame[kk]));
				strncpy(cache->frame[kk], argv[2], strlen(argv[2]));
			} // write to both cache and vhdd.
			else
			{
				replaceCache(t, argv[2]);
			}
	
			if (!fs_write(t, argv[2], strlen(argv[2])))
			{
				if (toFile)
					fprintf(out, "Write Success.\n");
				else
					puts ("Write Success.");
			}
			else
			{
				if (toFile)
					fprintf(out, "write Failed.\n");
				else
					puts ("write Failed.");
				return -1;
			}
			return 0;
		}

		if (strcmp (argv[0], "seek") == 0)
		{
			long long t = 0;
			int offset = 0;
			for (int i = 0; i < strlen(argv[1]); i++)
				t = t*10 + argv[1][i]-'0';
			for (int i = 0; i < strlen(argv[2]); i++)
				offset = offset*10 + argv[2][i]-'0';
			if (!fs_seek(t, offset))
			{
				if (toFile)
					fprintf(out, "Seek Success.\n");
				else 
					puts ("Seek Success.");
			}
			else
			{
				if (toFile)
					fprintf(out, "Seek failed.\n");
				else
					puts ("Seek failed.");
				return -1;
			}
			return 0;
		}
		if (strcmp (argv[0], "link") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p == dir_pwd)
			{
				if (toFile)
					fprintf(out, "Source file does not exist.\n");
				else
					puts ("Source file does not exist.");
				return -1;
			}
			p = getDirectory(argv[2]);
			if (p != dir_pwd)
			{
				if (toFile)
					fprintf(out, "Destination file has existed.");
				else
					puts ("Destination file has existed.");
				return -1;
			}
			if (!fs_link(argv[1], argv[2]))
			{
				if (toFile)
					fprintf(out, "Success link.\n");
				else
					puts ("Success link.");
			}
			else
			{
				if (toFile)
					fprintf(out, "Link error.\n");
				else
					puts ("Link error.");
				return -1;
			}
			return 0;
		}
		if (strcmp (argv[0], "cp") == 0)
		{
			DIRECTORY *p = getDirectory(argv[1]);
			if (p == dir_pwd)
			{
				if (toFile)
					fprintf(out, "Source file does not exist.\n");
				else
					puts ("Source file does not exist");
				return -1;
			}
			p = getDirectory(argv[2]);
			if (p != dir_pwd)
			{
				if (toFile)
					fprintf (out, "Destination file has existed.\n");
				else
					puts ("Destination file has existed.");
				return -1;
			}
			fs_open(argv[1], 1);
			char buf[10000];
			memset(buf, 0, sizeof(buf));
			fs_read(FD, buf, 10000);
			fs_close(FD);
			fs_open(argv[2], 2);
			if (!fs_write(FD, buf, strlen(buf)))
			{
				if (toFile)
					fprintf(out, "Copy Success.\n");
				else
					puts("Copy Success.");
			}
			else
			{
				if (toFile)
					fprintf(out, "Copy Failed.\n");
				else
					puts("Copy Failed.");
				return -1;
			}
			fs_close(FD);
			return 0;
		}
		if (strcmp (argv[0], "import") == 0)
		{
			FILE *fp = fopen (argv[1], "r");
			char buf[10000], ch;
			memset(buf, 0, sizeof(buf));
			int k = 0;
			while ((ch = fgetc(fp)) != EOF)
			{	if (ch == '\n')   	//Add \n
					ch = ' ';		
				buf[k++] = ch; 
			}
			fclose(fp);
			fs_open(argv[2], 2);
			if (!fs_write(FD, buf, k))
			{
				if (toFile)
					fprintf(out, "Import Success.\n");
				else
					puts("Import Success.");
			}
			else
			{
				if (toFile)
					fprintf(out, "Import Failed.\n");
				else
					puts ("Import Failed.");
				return -1;
			}
			fs_close(FD);
			return 0;
		}
		if (strcmp (argv[0], "export") == 0)
		{
			FILE *fp = fopen(argv[2], "w");
			char buf[10000];
			memset(buf, 0, sizeof(buf));
			fs_open(argv[1], 1);
			if (fs_read(FD, buf, 10000))
			{
				if (toFile)
					fprintf(out, "Export Failed.\n");
				else
					puts("Export Failed.");
			}
			//fs_close(FD);
			for (int i = 0; i < strlen(buf); i++) fputc(buf[i], fp);
			fclose(fp);
			if (toFile)
				fprintf(out, "Export Success.\n");
			else
				puts("Export Success.");
			return 0;
		}
		return 0;	
	}
	else if (argc == 5)
	{
		if (strcmp (argv[0], "sh") == 0 && strcmp (argv[1], "<") == 0 && strcmp (argv[3], ">") == 0)
		{
			out = fopen(argv[4], "w");
			toFile = true;
			parseCommand(argv[2]);
			fclose(out);
			toFile = false;
			return 0;
		}
		return 0;
	}
	else
	{
		if (toFile)
		{
			fprintf(out, "vShell Bash: <%s> command not found.\n", argv[0]);
			return 0;
		}
		else
		{
			printf("vShell Bash: <%s> command not found.\n", argv[0]);
			return 0;
		}
	}
	return 0;
}
