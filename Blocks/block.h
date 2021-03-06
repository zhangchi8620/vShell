#ifndef BLOCK_H_
#define BLOCK_H_

#include "../Types/types.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
* The block structure is the following:
* |in_use|vhddAddr|		content		|nextAddr| -> Structure.
* |  1   |   9    |     512     |   9    | -> Byte Costs.
*/

extern bool blockmap[MAX_BLOCK]; // This bitmap used to record the status of block.

int int2str(int x, char *str); // integers such as vhddAddr, nextAddr should be changed to a string in order to write in the vhdd.

int bool2str(int x, char *str); // blk->in_use is a bool variable, then change it to a string.

int blk2str(BLOCK *blk, char *str); // Change a whole block structure into a string pattern, preparing to write into vhdd.

int initBlock(int BlockNum); // Initiate the vhdd. BlockNum of blocks are formatted in the above structure. Default value is "~".

int str2int(char *str, int *x); 

int str2blk(char *str, BLOCK *blk);

int readBlock(int addr, char *content); // Given an address, read the content in the block. Empty block return -1. 

int writeBlock (int addr, char *content, int nextaddr); // Given an address, write data into the block. Used block is not permitted to write, until it is erased.

int eraseBlock (int addr); // Erase all infomation of the block in address addr.

int isEmpty(int addr); // Given an address of a block, then return whether it is availabe to use. 0: used, 1: empty, -1: error.

int findFreeBlock(int start);

int nextBlock(int addr);

#endif



