#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileDisk.h"

char *substring(char *string, int position, int length)
{
   char *pointer;
   int c;

   pointer = malloc(length+1);

   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(1);
   }

   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *(string+position-1);
      string++;
   }

   *(pointer+c) = '\0';

   return pointer;
}

int open(char* name){
	struct inode *currFile;
	struct inode newFile;
	int currFilePos;
	int newFilePos;
	int newDataPos;
	int *filePos;

	filePos = malloc(sizeof(int));
	currFilePos = name2file(name,filePos);
	if(currFilePos == -1){
		/*
		 *  file do not exist, create one
		 */
		newFile.type=iFILE;
		strncpy(newFile.name,name,MAX_NAME_LEN-1);
		newFile.name[9] = '\0';
		newFile.size = 0;
		newFile.status=fOPEN;
		newFile.seek_ptr = 0;
		newFilePos = nextFreeBlock(0);
		newDataPos = nextFreeBlock(1);
		newFile.direct[0] = newDataPos;
		writeInode(newFilePos,&newFile);
		setBit(newFilePos,1);
		setBit(newDataPos,1);
		addElement(newFilePos,-1,-1);
		free(filePos);
		printf("open create a new file success \n");
		return 0;
	}
	else{
		currFile = malloc(sizeof(struct inode));
		readInode(currFilePos,currFile);
		currFile->status = fOPEN;
		writeInode(currFilePos,currFile);
		free(filePos);
		free(currFile);
		printf("open exist file success \n");
		return 0;
	}

}

int close(char* name){
	struct inode *currFile;
	int currFilePos;
	int *filePos;

	filePos = malloc(sizeof(int));
	currFilePos = name2file(name,filePos);
	if(currFilePos == -1){
		return -1;
	}
	else{
		currFile = malloc(sizeof(struct inode));
		readInode(currFilePos,currFile);
		currFile->status = fCLOSE;
		writeInode(currFilePos,currFile);
		printf("close exist file success \n");
		free(filePos);
		free(currFile);
		return 0;
	}
}

int rm (char* name){
	struct sb *superBlock;
	struct inode *currDirectory;
	struct inode *currFile;
	int j;
	int i;
	int currFilePos;
	int currDataPos;
	int *filePos;

	filePos = malloc(sizeof(int));
	currFilePos = name2file(name,filePos);
	if(currFilePos == -1){
		return -1;
	}
	else{
		setBit(currFilePos,0);
		for(i=0;i<DATA_NUM ;i++){
			if(currFile->direct[i] != 0){
				currDataPos = currFile->direct[i];
				setBit(currDataPos,0);
			}
		}
		redElement(currFilePos,1,i);
		printf("delete exist file success \n");
		return 0;

	}

}


int main(){
	char* result = malloc(80);
	char* result_1 = malloc(80);
	char* result_2 = malloc(80);
	my_mkfs();
	/*
	open("foo.txt");
	if(read("foo.txt",result,100) == 1){
		printf("should not fail read foo \n");
	}
	printf("The result of read foo should be nothing: %s \n",result);
	if(close("foo.txt") == 1){
		printf("should not fail close foo \n");
	}
	if(read("foo.txt",result,100) == 1){
		printf("Read should fail because file is close.\n");
	}
	open("foo.txt");
	if(write("foo.txt","hello\0",5) == 1){
		printf("should not fail write foo \n");
	}
	if(read("foo.txt",result,100) == 1){
		printf("Read should fail because file is close.\n");
	}
	printf("The result of read foo should be hello: %s \n",result);
	if(close("foo.txt") == 1){
		printf("should not fail close foo \n");
	}
	if(write("good.txt","good\0",4) == 1){
		printf("should not fail write good \n");
	}
	open("good.txt");
	if(read("good.txt",result_1,100) == 1){
		printf("should not fail read good \n");
	}
	printf("The result of read good should be good: %s \n",result_1);
	if(close("good.txt") == 1){
		printf("should not fail close foo \n");
	}
	*/
	open("good.txt");
	/*
	if(write("good.txt","good\0",4) == 1){
		printf("should not fail write good \n");
	}
	seek("good.txt",2);
	if(read("good.txt",result_2,100) == 1){
		printf("should not fail read ood \n");
	}
	printf("The result of read good should be good: %s \n",result_2);
	 * */
	if(close("good.txt") == -1){
		printf("should not fail close good \n");
	}
	open("good.txt");
	if(close("good.txt") == -1){
		printf("should not fail close good \n");
	}
	if(rm("good.txt") == -1){
	   	printf("should not fail rm good \n");
	}
	if(close("good.txt") == 0){
		printf("should not success close good \n");
	}
	if(rm("good.txt") == 0){
	   	printf("should not successrm good \n");
	}
	return 0;
}


