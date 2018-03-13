/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "block.h"
#include "fileDisk.h"

char *disk;

void initDisk() {
	disk = malloc(BLOCK_SIZE*TOTAL_BLOCK_NUM);
}

void readInode(int blockNum, void *inode) {
    	memcpy(inode, disk+blockNum*BLOCK_SIZE, sizeof(struct inode));
}

void writeInode(int blockNum, void *inode) {
	memmove(disk+blockNum*BLOCK_SIZE, inode, sizeof(struct inode));
}

void readSeekData(int blockNum,int seek_ptr,void* data,int size){
	memcpy(data,disk+blockNum*BLOCK_SIZE+seek_ptr,size);
}

void writeSeekData(int blockNum, int seek_ptr, void* data, int size){
	memmove(disk+blockNum*BLOCK_SIZE+seek_ptr, data, size);
}

int closeDisk() {
	free(disk);
	return 0;
}

void getSuper(void *superBlock) {
	memcpy(superBlock, disk, sizeof(struct sb));
}

void writeSuper(void *superBlock) {
	memcpy(disk, superBlock, sizeof(struct sb));
}


int my_mkfs() {
	int i;
	char *name;
	struct sb superBlock;
	struct inode root;

	initDisk();

	/* Initial super block */
	superBlock.numDataBlock = DATA_NUM;
	superBlock.numInodeBlock = INODE_NUM;
	superBlock.numFreeDataBlock = DATA_NUM;
	superBlock.numFreeInodeBlock = INODE_NUM;
	superBlock.inodeStart = 2;		/* still need to decide */
	superBlock.dataStart = superBlock.inodeStart+INODE_NUM;		/* still need to decide */

	for (i = 0; i < TOTAL_BLOCK_NUM; i++) {
		superBlock.bitMap[i] = 0;	/* all unused */
	}
	superBlock.bitMap[0] = 1; 		/* superBlock */
	superBlock.bitMap[1] = 1; 		/* root directory */

	writeSuper(&superBlock);

	/* Initial root directory */
	name = "/";
	root.type = iDIRECT;
	strncpy(root.name, name, sizeof(name)-1);
	root.name[MAX_NAME_LEN-1] = '\0';
	root.size = 0;	/* have no element inside it */
	for (i = 0; i < DIRECT_NUM; i++) {
		root.direct[i] = 0;	/* all unused */
	}

	writeInode(1, &root);
	return 1;
}

/* find next free block: 0 for inode, 1 for data */
int nextFreeBlock(int choice) {
	int i, start, end;
	struct sb *superBlock = malloc(sizeof(struct sb));

	getSuper((void*)superBlock);
	if (choice == 0) {
		start = superBlock->inodeStart;
		end = start + superBlock->numInodeBlock;
	} else {
		start = superBlock->dataStart;
		end = start + superBlock->numDataBlock;
	}
	for (i = start; i < end; i++) {
		if(superBlock->bitMap[i]==0)
			break;
	}
	if (i == end) {
		return -1;
	}

	free(superBlock);

	return i;
}
/*
 * update the information of current directory and super block after adding
 * blockNum: inode index that is gonna to add to current directory
 * inode: change of free inode (must be negative)
 * data: change of data (must be negative)
 * */
int addElement(int blockNum, int inode, int data, int curBlockNum) {
	int i;
	struct sb *superBlock = malloc(sizeof(struct sb));
	struct inode *direct = malloc(sizeof(struct inode));

	getSuper(superBlock);
	readInode(curBlockNum, direct);
	/* update current directory child information*/
	for (i = 0; i < DATA_NUM; i++) {
		if (direct->direct[i] == 0) {
			direct->direct[i] = blockNum;
			break;
		}
	}
	direct->size++;
	writeInode(curBlockNum, (void*)direct);

	/* update super block info */
	superBlock->numFreeInodeBlock = superBlock->numFreeInodeBlock + inode;
	superBlock->numFreeDataBlock = superBlock->numFreeDataBlock + data;
	writeSuper((void*)superBlock);

	free(superBlock);
	free(direct);

	return 0;
}

/*
 * update the information of current directory after reducing
 * size--; direct[dirIndex]=0;
 */
int redElement(int dirIndex, int inode, int data, int curBlockNum) {
	struct sb *superBlock = malloc(sizeof(struct sb));
	struct inode *direct = malloc(sizeof(struct inode));

	getSuper(superBlock);
	readInode(curBlockNum, direct);
	/* update current directory information*/
	direct->size--;
	direct->direct[dirIndex] = 0;
	writeInode(curBlockNum, (void*)direct);

	/* update super block info */
	superBlock->numFreeInodeBlock = superBlock->numFreeInodeBlock + inode;
	superBlock->numFreeDataBlock = superBlock->numFreeDataBlock + data;
	writeSuper((void*)superBlock);

	free(superBlock);
	free(direct);

	return 0;
}

/* update the bitmap of superBlock after adding or deleting */
int setBit(int blockNum, int bit) {
	struct sb *superBlock = malloc(sizeof(struct sb));
        getSuper(superBlock);

	superBlock->bitMap[blockNum] = bit;
	writeSuper((void*)superBlock);

	free(superBlock);

	return 0;
}

/* get the bitmap of superBlock to check if file exist */
int getbit(int blockNum) {
	int result;
	struct sb *superBlock = malloc(sizeof(struct sb));
        getSuper(superBlock);
	result = superBlock->bitMap[blockNum];
	free(superBlock);
	return result;
}


int name2dir(char *name, int *directIndex, int curBlockNum) {
	int i, j;
	struct inode *curDir = malloc(sizeof(struct inode));		/* current directory */
	struct inode *anode = malloc(sizeof(struct inode));		/* an inode in directory */

	readInode(curBlockNum, curDir);

	/* loop through to find name match parameter and is a directory */
	for(i = 0, j = 0; j < DATA_NUM; j++){
		if(i == curDir->size)		/* have checked all child */
			break;
		if(curDir->direct[j] == 0)
			continue;
		i++;
		readInode(curDir->direct[j], anode);
		/* check if file name match parameter and it is a directory */
		if(0 == strcmp(anode->name, name) && anode->type == iDIRECT){
			*directIndex = j;

			free(curDir);
			free(anode);

			return curDir->direct[j];
		}
	}

	free(curDir);
	free(anode);

	return -1;
}

int name2file(char *name, int *directIndex, int curBlockNum) {
	int i, j;
	struct inode *curDir = malloc(sizeof(struct inode));		/* current directory */
	struct inode *anode = malloc(sizeof(struct inode));		/* an inode in directory */

	readInode(curBlockNum, curDir);

	/* loop through to find name match parameter and is a directory */
	for(i = 0, j = 0; j < DATA_NUM; j++){
		if(i == curDir->size)		/* have checked all child */
			break;
		if(curDir->direct[j] == 0)
			continue;
		i++;
		readInode(curDir->direct[j], anode);
		/* check if file name match parameter and it is a directory */
		if(0 == strcmp(anode->name, name) && anode->type == iFILE){
			*directIndex = j;

			free(curDir);
			free(anode);

			return curDir->direct[j];
		}
	}

	free(curDir);
	free(anode);

	return -1;
}

