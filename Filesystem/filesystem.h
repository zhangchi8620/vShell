#ifndef FILESYSTM_H
#define FILESYSTM_H

#include "../Blocks/block.h"
#include "../Types/types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern DIRECTORY *dir_root;
extern DIRECTORY *dir_pwd;
extern char fbuffer[10000];
extern long long FD;
extern CACHE *cache;
extern bool OpenNewFile, OpenNewFile_s;
extern int FLAGS;
extern int FileAddr;
extern int f_size;
extern int d_size;

int initCache();

int refreshCache();

int replaceCache(long long fd, char *frame);

int checkCache(long long fd);

int clearCache(int k);

int buildRoot();

int initDirectoryTree (); // create the directory tree from vhdd to memory.

int updateDirectory (DIRECTORY *dir); // update directory tree.

int updateDirectoryTree (); // update directory tree.

int retrieveRoot(DIRECTORY *root);

int retrievePwd(DIRECTORY *pwd);

DIRECTORY *getDirectory (char *path); // Search "path" in the directory tree, then return the DIRECTORY.

//int getDirectory (DIRECTORY *p, char *path);

int buildDirectoryTree(DIRECTORY *parent);

int fs_mkfs (void); // Format the filesystem in the Virtual Drive (vhdd).

int fs_open (char *filename, int flags); // Open the file: "filename", using "flags" method, e.g. fs_open ("/novel.txt", FS_O_RDONLY/FS_O_WRONLY/FS_O_RDWR);

int fs_close (long long fd); // Close the file: fd.

int fs_read (long long fd, char *buf, int count); // Read "fd" for "count" size into "buf".

int fs_write (long long fd, char *strinput, int count); // Write "fd" for "count" size into "buf".

int fs_seek (long long fd, int offset); // Move file pointer from "fd" to "fd+offset" and return the new "fd".
int fs_stat (char *filename); //Return information about a file.

int fs_mkdir (char *dirname); // Create a new directory. If the dirname exists, this function should failed.

int fs_rmdir (char *dirname); // return 0 when successfully delete an empty directory; return -1 when delete a non-empty directory or dirname failed.

int fs_chdir (char *newdirname); // Change the current dirname to the new dirname. return 0: success; return -1: failed.

int fs_link (char *oldpath, char *newpath); // Hard link. Newpath will not be overwritted if exists. return 0: success; return -1: failed. operate on inode.

int fs_unlink (char *filename); // Delete name from directory tree. If the last name, delete the inode.

int trimDirname(char *dirname, char *trimname);

int getFullPath (DIRECTORY *dir, char *rt);

int fs_tree (DIRECTORY *p, int cnt);

int fs_stat (char *filename); // Return filname's properties to buf.

#endif
