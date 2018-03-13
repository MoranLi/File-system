/*
Names: Yukun Li, Yuhan Hu 
NSID : yul040, yuh347
Std# : 11173592, 11212253
Group: group07
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileDisk.h"
#include "block.h"




int my_mkdir(char *name, struct threadInfo *info) {
	int i, blockNum, curBlockNum;
	struct inode *anode;
	struct sb *superBlock;

	curBlockNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curBlockNum)) {
		printf("ERR(mkdir): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}

	if(name2dir(name, &i, curBlockNum) > -1) {
		printf("ERR: mkdir[%s] exists\n", name);
		return -1;
	}

	/*TODO StartWrite()*/

	anode = malloc(sizeof(struct inode));
	superBlock = malloc(sizeof(struct sb));

	getSuper(superBlock);

	if(superBlock->numFreeInodeBlock == 0) {
		printf("Sorry, we only support 16 inode\n");
		goto rt;
	}
	/* initial and write inode */
	anode->type = iDIRECT;
	anode->size = 0;
	blockNum = nextFreeBlock(0);	/* get a free inode block */
	strncpy(anode->name, name, sizeof(name)-1);
	anode->name[MAX_NAME_LEN-1] = '\0';	/* copy name */
	getSuper(superBlock);
	anode->parent = curBlockNum;

	for (i = 0; i < DIRECT_NUM; i++) {
		anode->direct[i] = 0;	/* all unused */
	}
	writeInode(blockNum, anode);

	/* add one element to current directory */
	addElement(blockNum, -1, 0, curBlockNum);

	/* update the bit of this block*/
	setBit(blockNum, 1);
rt:
	free(anode);
	free(superBlock);

	return 0;
}

int my_cd(char *name, struct threadInfo *info) {
	int dirIndex, blockIndex;
	struct inode *curDir = malloc(sizeof(struct inode));

	/* if current directory not exist */
	if(0 == getbit(info->curBlockNum)) {
		printf("ERR(mkdir): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}


	readInode(info->curBlockNum, curDir);

	/* if it's ".." */
	if(0 == strcmp(name, "..")) {
		if(info->curBlockNum != 1) {
			info->curBlockNum = curDir->parent;
		}
		goto rt;
	} else {
		blockIndex = name2dir(name, &dirIndex, info->curBlockNum);
	}
	/* if not exist */
	if(-1 == blockIndex) {
		printf("Direct %s doesn't exist\n", name);
		return -1;
	}
	info->curBlockNum = blockIndex;
rt:
	free(curDir);

	return blockIndex;
}

int my_rmdir(char *name, struct threadInfo *info) {
	int dirIndex, blockIndex, curDirNum;
 	struct inode *dir = malloc(sizeof(struct inode));

	curDirNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(mkdir): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}

	blockIndex = name2dir(name, &dirIndex, curDirNum);
	/* if not exist */
	if(-1 == blockIndex) {
		printf("Direct %s doesn't exist\n", name);
		return -1;
	}
	readInode(blockIndex, dir);
	if(dir->size != 0) {
		printf("Fail: [%s] is not empty\n", name);
		return -1;
	}

	/* reduce one element from current directory */
	redElement(dirIndex, 1, 0, curDirNum);

	/* update the bit of this block*/
	setBit(blockIndex, 0);

	free(dir);

	return 0;
}

int my_open(char* name, struct threadInfo *info){
	struct inode newFile;
	int currFilePos;
	int newFilePos;
	int newDataPos;
	int *filePos;
	int i;
	int curDirNum;

	curDirNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(read): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}

	filePos = malloc(sizeof(int));
	currFilePos = name2file(name, filePos, curDirNum);
	if(currFilePos == -1){
		/*
		 *  file do not exist, create one
		 */
		newFile.type=iFILE;
		strncpy(newFile.name,name,MAX_NAME_LEN-1);
		newFile.name[9] = '\0';
		newFile.size = 0;
		newFilePos = nextFreeBlock(0);
		newDataPos = nextFreeBlock(1);
		for(i=0;i<DATA_NUM;i++){
			newFile.direct[i] = 0;
		}
		newFile.direct[0] = newDataPos;
		writeInode(newFilePos,&newFile);
		setBit(newFilePos,1);
		setBit(newDataPos,1);
		addElement(newFilePos,1,1, curDirNum);
		info->openbit[newFilePos] = 1;
		free(filePos);
		printf("Descriptor for %s: %d\n", name, newFilePos);
		return newFilePos;
	}
	else{
		info->openbit[currFilePos] = 1;
		printf("Descriptor for %s: %d\n", name, currFilePos);
		return currFilePos;
	}
}


int my_close(int fd, struct threadInfo *info){
	struct inode *curDir;
	int i, curDirNum;

	curDirNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(seek): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}
	/* if fd not in current directory */
	curDir = malloc(sizeof(struct inode));
	readInode(curDirNum, curDir);
	for (i = 0; i < DATA_NUM+2; i++) {
		if (curDir->direct[i] == fd)
			break;
	}
	if (i == DATA_NUM) {
		printf("ERR(seek): file doesn't exist\n");
		return -1;
	}
	/* if file is not open */
	if (info->openbit[fd] == 0) {
		printf("ERR(seek): file is closed\n");
		return -1;
	}
	info->openbit[fd] = 0;
	return 0;
}


int my_rm (char* name, struct threadInfo *info){

	struct inode *currFile;
	int i;
	int *filePos;
	int currFilePos;
	int currDataPos;
	int curDirNum;

	curDirNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(read): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}

	filePos = malloc(sizeof(int));
	currFilePos = name2file(name, filePos, curDirNum);

	if(currFilePos == -1) {
		printf("ERR(rm)[%s] doesn't exist\n", name);
		free(filePos);
		return -1;
	}

	if(getbit(currFilePos) == 1){
		currFile = malloc(sizeof(struct inode));
		readInode(currFilePos,currFile);
		setBit(currFilePos,0);
		for(i=0;i<DATA_NUM ;i++){
			if(currFile->direct[i] != 0){
				currDataPos = currFile->direct[i];
				setBit(currDataPos,0);
			}
		}
		name2file(currFile->name,filePos, curDirNum);
		redElement(*filePos,1,i, curDirNum);
		info->openbit[currFilePos] = 0;
		free(currFile);
		return 0;
	}
	return -1;

}

int my_seek(int fd, int seek_ptr, struct threadInfo *info){
	struct inode *curDir;
	int i, curDirNum;

	curDirNum = info->curBlockNum;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(seek): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		return -1;
	}
	/* if fd not in current directory */
	curDir = malloc(sizeof(struct inode));
	readInode(curDirNum, curDir);
	for (i = 0; i < DATA_NUM+2; i++) {
		if (curDir->direct[i] == fd)
			break;
	}
	if (i == DATA_NUM) {
		printf("ERR(seek): file doesn't exist\n");
		return -1;
	}
	/* if file is not open */
	if (info->openbit[fd] == 0) {
		printf("ERR(seek): file is closed\n");
		return -1;
	}
	info->seek_ptr[fd] = seek_ptr;
	return 0;
}

int my_read(int fd,char* data, struct threadInfo *info){
	struct inode *currFile;
	struct inode *curDir;
	int i, curDirNum, length, seek_ptr;

    
	curDirNum = info->curBlockNum;
	seek_ptr = info->seek_ptr[fd];
	length = BLOCK_SIZE-seek_ptr;

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(read): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		goto fail;
	}
	/* if fd in current directory */
	curDir = malloc(sizeof(struct inode));
	readInode(curDirNum, curDir);
	for (i = 0; i < DATA_NUM+2; i++) {
		if (curDir->direct[i] == fd)
			break;
	}
	/* if file doesn't exist */
	if (i == DATA_NUM) {
		printf("ERR(read): file doesn't exist\n");
		goto fail;
	}
	/* if file is not open */
	if (info->openbit[fd] == 0) {
		printf("ERR(read): file is closed\n");
		goto fail;
	}

	if(getbit(fd) == 1){
		currFile = malloc(sizeof(struct inode));
		readInode(fd,currFile);
		if(currFile->size != 0){
			readSeekData(currFile->direct[0], seek_ptr,data,length);
			free(currFile);
			return 0;
		}
		free(currFile);
		goto fail;
	}
	
fail:	
	return -1;
}

int my_write(int fd, char* data, struct threadInfo *info){
	struct inode *currFile;
	struct inode *curDir;
	int i, curDirNum, length, seek_ptr;
    

	curDirNum = info->curBlockNum;
	seek_ptr = info->seek_ptr[fd];
	length = strlen(data);

	/* if current directory not exist */
	if(0 == getbit(curDirNum)) {
		printf("ERR(write): current directory doesn't exist, Go to root\n");
		info->curBlockNum = 1;
		goto fail;
	}
	/* if fd in current directory */
	curDir = malloc(sizeof(struct inode));
	readInode(curDirNum, curDir);
	for (i = 0; i < DATA_NUM+2; i++) {
		if (curDir->direct[i] == fd)
			break;
	}
	/* if file doesn't exist */
	if (i == DATA_NUM) {
		printf("ERR(write): file doesn't exist\n");
		goto fail;
	}
	/* if file is not open */
	if (info->openbit[fd] == 0) {
		printf("ERR(write): file is closed\n");
		goto fail;
	}

	if(getbit(fd) == 1){
		currFile = malloc(sizeof(struct inode));
		readInode(fd,currFile);
		writeSeekData(currFile->direct[0],seek_ptr,data,length);
		currFile->size = seek_ptr+strlen(data);
		writeInode(fd,currFile);
		free(currFile);
		return 0;
	}
fail:
	return -1;
}
