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
#include "my_fs.h"

int BLOCK_SIZE = 1024;

struct threadInfo *info;

void printSuper(int curBlockNum) {
	int i;
	struct sb *superBlock;
       	struct inode *curDir;

	superBlock = malloc(sizeof(struct sb));
	curDir = malloc(sizeof(struct inode));

	getSuper((void*)superBlock);
	readInode(curBlockNum, (void*)curDir);

	printf("*****Test for superBlock attribute*****\n");
	printf("numDataBlock: %d\n", superBlock->numDataBlock);
	printf("numFreeDataBlock: %d\n", superBlock->numFreeDataBlock);
	printf("dataStart: %d\n", superBlock->dataStart);

	printf("numInodeBlock: %d\n", superBlock->numInodeBlock);
	printf("numFreeInodeBlock: %d\n", superBlock->numFreeInodeBlock);
	printf("inodeStart: %d\n", superBlock->inodeStart);

	printf("bitMap:\n");
	for(i = 0; i < DATA_NUM; i++)
		printf("%d, ", superBlock->bitMap[i]);
	printf("\n");

	printf("current directory child array\n");
	for(i = 0; i < DATA_NUM; i++)
		printf("%d, ", curDir->direct[i]);
	printf("\n");

	free(superBlock);
	free(curDir);
}

void printbit() {
	int i;
	printf("open bit: \n");
	for (i = 0; i < INODE_NUM; i++) {
		printf("%d, ", info->openbit[i]);
	}
	printf("\nseek bit: \n");
	for (i = 0; i < INODE_NUM; i++) {
		printf("%d, ", info->seek_ptr[i]);
	}
	printf("\n");
}

void printCurDir(int curBlockNum) {
       	int i, j;
	struct inode *curDir = malloc(sizeof(struct inode));
	struct inode *anode = malloc(sizeof(struct inode));
	readInode(curBlockNum, curDir);

	printf("We are in: %s\n", curDir->name);

	for(i = 0, j = 0; j < DATA_NUM; j++){
		if(i == curDir->size)		/* have checked all child */
			break;
		if(curDir->direct[j] == 0)
			continue;
		i++;
		readInode(curDir->direct[j], anode);
		printf("\t%s\t%d\n", anode->name, anode->type);
	}
	/*
	printSuper(curBlockNum);
	printbit();
	*/
}

void printNext() {
	printf("*****Test for next free\n");
	printf("next free inode: %d\n", nextFreeBlock(0));
	printf("next free data: %d\n\n", nextFreeBlock(1));
}



#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

static int getLine(char *buff, size_t sz) {
	int ch, extra;

	if(fgets(buff, sz, stdin) == NULL)
		return NO_INPUT;
	if(buff[strlen(buff)-1] != '\n') {
		extra = 0;
		while(((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? TOO_LONG : OK;
	}
	buff[strlen(buff)-1] = '\0';
	return OK;
}



void init() {
	int i;
	info = malloc(sizeof(struct threadInfo));
	for (i = 0; i < INODE_NUM; i++) {
		info->openbit[i] = 0;
		info->seek_ptr[i] = 0;
	}
}



int run(char *cmd, char *name, int anum, char* data) {
	if(0 == strcmp(cmd, "mkdir")) {

		my_mkdir(name, info);

	} else if(0 == strcmp(cmd, "cd")) {

		my_cd(name, info);

	} else if(0 == strcmp(cmd, "open")) {

		my_open(name, info);

	} else if(0 == strcmp(cmd, "close")) {

		my_close(atoi(name), info);

	} else if(0 == strcmp(cmd, "read")) {

		char buff[1024];
		if(my_read(atoi(name), buff, info)>-1)
			printf("%s\n",buff);

	} else if(0 == strcmp(cmd, "write")) {

		my_write(atoi(name), data, info);

	} else if(0 == strcmp(cmd, "seek")) {

		my_seek(atoi(name), anum, info);

	} else if(0 == strcmp(cmd, "rm")) {

		my_rm(name, info);

	} else if(0 == strcmp(cmd, "rmdir")) {

		my_rmdir(name, info);
	
	} else if(0 == strcmp(cmd, "ls")) {
	
		printCurDir(info->curBlockNum);
	
	}else {
	
		printf("wrong input[%s %s]\n", cmd, name);
	
	}
	return 0;
}


int main() {
	int rc;
	char* name;
	char* pos;
	char* data;
	char buff[100];

	init();
	info->curBlockNum = my_mkfs();

	for(;;) {
		rc = getLine(buff, sizeof(buff));
		if(rc == NO_INPUT)
			printf("No input\n");
		else if(rc == TOO_LONG)
			printf("Input too long [%s]\n", buff);
		else if(0 == strcmp(buff, "quit"))
			return 0;
		else {
			strtok(buff, " ");
			name = strtok(NULL, " ");
			if (0 != strcmp(buff, "ls") && NULL == name) {
				printf("Please enter file name\n");
			} else if (0 == strcmp(buff,"seek")){
				pos = strtok(NULL," ");
				if(pos == NULL){
					printf("Please enter the seek point\n");
				} else{
					run(buff,name,atoi(pos),NULL);
				}
			} else if (0 == strcmp(buff,"write")){
				data = strtok(NULL," ");
				if(data == NULL){
					printf("Please enter the data and the length\n");
				} else{
					run(buff, name, strlen(data), data);
				}
			} else if (0 == strcmp(buff,"read")) {
					run(buff,name,atoi(pos),NULL);
			/* mkdir rmdir open close (rm)*/
			} else {
				run(buff, name, 0,NULL);
			}
		}
	}
	return 0;
}
