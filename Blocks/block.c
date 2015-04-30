#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Types/types.h"
#include "../Blocks/block.h"

bool blockmap[MAX_BLOCK];
extern bool WriteFile, OpenNewFile;
int int2str(int x, char *str)
{
	char a[10];
	memset(a, 0, sizeof(a));
	memset(str, 0, sizeof(str));
	for (int i = 0; i < 10; i++)
		a[i] = str[i] = '~'; // Default value is ~. 
	int k = 0;
	while (x)
	{
		a[k++] = x%10 + '0';
		x /= 10;
	}
	for (int i = 0; i < k; i++)
		str[i] = a[k-i-1];
	str[9] = '\0';
	return 0;
}

int bool2str(int x, char *str)
{
	if (x == 0)
		str[0] = '0', str[1] = '\0';
	else
		str[0] = '1', str[1] = '\0';
	return 0;	
}

int blk2str(BLOCK *blk, char *str)
{
	char tmp[10];
	memset(str, 0, sizeof(str));
	bool2str(blk->in_use, tmp);
	strncpy(str, tmp, 1);
	int2str(blk->vhddAddr, tmp);
	strncpy(str+1, tmp, 9);
	strncpy(str+10, blk->content, 512);
	int2str(blk->nextAddr, tmp);
	strncpy(str+522, tmp, 9);
	//printf("write: %s\n", str);
	return 0;
}

int initBlock(int BlockNum) // initialize the overal VHDD, including the directory & block tree parts.
{
	memset(blockmap, 0, sizeof(blockmap));
	FILE *fp = fopen("vhdd", "w+");
	if (fp == NULL) return -1;
	for (int i = 1; i <= BlockNum; i++)
	{
		BLOCK *blk = (BLOCK *)malloc(BLOCK_SIZE);
		blk->vhddAddr = i;
		blk->in_use = 0;
		blk->nextAddr = i;
		memset(blk->content, '~', 512);
		blk->content[512] = '\0';
		char str[BLOCK_SIZE+1];
		memset(str, 0, sizeof(str));
		blk2str(blk, str);
		str[BLOCK_SIZE] = '\0';
		fputs(str, fp);
		free(blk);
		//printf("%d\n", i);
	}
	fclose(fp);
	return 0;
}

int str2int(char *str, int *x)
{
	(*x) = 0;
	for (int i = 0; i < strlen(str); ++i)
	{
		if (str[i] == '~') break;
		(*x) = (*x)*10 + str[i]-'0';
	}
	return (*x);
}

int str2blk(char *str, BLOCK *blk)
{
	blk->in_use = str[0] - '0';
	char tmp[10];
	memset(tmp, 0, sizeof(tmp));
	strncpy(tmp, str+1, 9);
	str2int(tmp, &blk->vhddAddr);
	strncpy(blk->content, str+10, 512);
	blk->content[512] = '\0';
	strncpy(tmp, str+522, 9);
	str2int(tmp, &blk->nextAddr);
	return 0;
}

int readBlock(int addr, char *content)
{
	
//	if (WriteFile == true)
//		blockmap[addr] = true;			
	if (blockmap[addr] == false) return -1;
	FILE *f = fopen("vhdd", "r");
	if (f == NULL) return -1;
	fseek(f, (addr-1)*BLOCK_SIZE, 0);
	char str[BLOCK_SIZE];
	fgets(str, BLOCK_SIZE, f);
	BLOCK b;		
	str2blk(str, &b);
	if (b.in_use == 0) return -1;
	strncpy(content, b.content, strlen(b.content));
	content[strlen(b.content)] = '\0';
	fclose(f);
	return 0;
}
	
int writeBlock (int addr, char *content, int nextaddr)
{
	if (blockmap[addr] == true) return -1; 
	if (strlen(content) > 512)
	{
		return -1;	
	}
	FILE *f = fopen("vhdd", "r+");
	if (f == NULL) return -1;
	fseek(f, (addr-1)*BLOCK_SIZE, 0);
	char str[BLOCK_SIZE+1];
	BLOCK *b = (BLOCK *)malloc(sizeof(BLOCK));
	b->vhddAddr = addr;
	b->nextAddr = nextaddr;
	b->in_use = 1;
	memset(b->content, '~', sizeof(b->content));	
	strncpy(b->content, content, strlen(content));
	b->content[512] = '\0';
	blk2str(b, str);
	str[BLOCK_SIZE] = '\0';
	//printf("write: %s\n", str);
	fseek(f, (addr-1)*BLOCK_SIZE, 0);
	fprintf(f, "%s", str);
	blockmap[addr] = true;
	fclose(f);
	free(b);
	return 0;
}

int eraseBlock (int addr)
{
	if (blockmap[addr] == false) return -1;
	FILE *f = fopen("vhdd", "r+");
	if (f == NULL) return -1;
	fseek(f, (addr-1)*BLOCK_SIZE, 0);
	fputc('0', f);
	blockmap[addr] = false;
	fclose(f);
	return 0;
}	

int isEmpty(int addr)
{
	FILE *f = fopen("vhdd", "r");
	if (f == NULL) return -1;
	fseek(f, (addr-1)*BLOCK_SIZE, 0);
	int in_use = fgetc(f) - '0';
	fclose(f);
	printf("%d\n", in_use);
	if ((in_use == 0) && (blockmap[addr] == false)) return 1;
	else if ((in_use == 1) && (blockmap[addr] == true)) return 0;
	else return -1;
}

int initBlockMap(int nn)
{
	FILE *f = fopen("vhdd", "r");
	if (f == NULL) return -1;
	for (int i = 1; i <= nn; i++)
	{	
		blockmap[i] = fgetc(f) - '0';
		fseek(f, BLOCK_SIZE-1, 1);
	}
	fclose(f);
	return 0;
}

int findFreeBlock(int start) // for write.
{
	for (int i = start; i < MAX_BLOCK; ++i)
		if(blockmap[i] == 0) return i;
	return -1;
}

int nextBlock(int addr) // for read.
{
	char content[1024];
	FILE *f = fopen("vhdd", "r");
	if (f == NULL) return -1;
	int n = addr-1;
	while (n--)
		fseek(f, BLOCK_SIZE, 1);
	memset(content, 0, sizeof(content));
	fgets(content, BLOCK_SIZE, f);
	char t[10];
	memset(t, 0, sizeof(t));
	strncpy(t, content+522, 9);
	int ans = 0;
	str2int(t, &ans);
	return ans;
}
