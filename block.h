/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/


#ifndef BLOCK_H
#define BLOCK_H

extern int BLOCK_SIZE;
#define TOTAL_BLOCK_NUM 64
#define INODE_NUM 16
#define DATA_NUM TOTAL_BLOCK_NUM-INODE_NUM-1-1	/* superblock and root */
#define MAX_NAME_LEN 20
#define DIRECT_NUM DATA_NUM

enum inodeType {iFILE, iDIRECT};

struct inode {
	enum inodeType type;
	int parent;
	char name[MAX_NAME_LEN];
	int size;
	int direct[DATA_NUM];
};

struct sb {
	int numDataBlock;
	int numInodeBlock;
	int numFreeDataBlock;
	int numFreeInodeBlock;
	int inodeStart;			/* start block # for inode */
	int dataStart;			/* start block # for data */
	int bitMap[TOTAL_BLOCK_NUM];  	/* 1 for use, 0 for empty */
};

struct threadInfo {
	int curBlockNum;
	int openbit[INODE_NUM];
	int seek_ptr[INODE_NUM];
};
#endif

