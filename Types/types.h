#ifndef TYPES_H
#define TYPES_H

#define FS_O_RDONLY 1
#define FS_O_WRONLY 2
#define FS_O_RDWR 3

#define BASEOFFSET 1000

#define BLOCK_SIZE 531 // 1+9+512+9

#define MAX_BLOCK 65535

struct BLOCK 
{
	int vhddAddr; 
	int nextAddr;
	int in_use;
	char content[513]; // The default value is ~.
};

struct INODE
{
	int vhddAddr;
	int nBlocks;
	int startAddr; // Only need to know the starting block.
	int inode_size;
};

struct DIRECTORY
{
	int inode; // 1: inode; 0: dir
	int nchild; // 200 = no child.
	int vhddAddr; // the address that stored in vhdd.
	DIRECTORY *parent;
	DIRECTORY *child[1024]; //Max child: 1024
	int childAddr[1024];
	INODE *inode_ptr; // NULL represent directory.
	char dirName[256]; // Max name length: 255
	char modify_date[25]; // "Sat May 20 17:36:17 2000"
	char control[10]; //"-rw-r--r--+"
	int dir_size;
	int nlink;
	int nBlocks;
};


struct CACHE
{
	long long fd[4];
	char frame[4][10000];
	bool used[4];
	int count[4];
};

#endif
