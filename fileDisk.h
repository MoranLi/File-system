/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/


#ifndef FILEDISK_H
#define FILEDISK_H
#include "block.h"




int my_mkfs();

/* function for inode */
void readInode(int blockNum, void *inode);
void writeInode(int blockNum, void *inode);

/* function for directory */
int name2dir(char *name, int *directIndex, int curBlockNum);

/* functions for super */
void getSuper(void *superBlock);
void writeSuper(void *superBlock);

int setBit(int blockNum, int bit);
int nextFreeBlock(int choice);
int redElement(int dirIndex, int inode, int data, int curBlockNum);
int addElement(int blockNum, int inode, int data, int curBlockNum);

int getbit(int blockNum);

/* functions for file*/
int name2file(char *name, int *directIndex, int curBlockNum);

/*functions for data*/
void readSeekData(int blockNum,int seek_ptr,void* data, int size);
void writeSeekData(int blockNum, int seek_ptr, void* data, int size);
#endif
