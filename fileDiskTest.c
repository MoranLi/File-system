#include <stdio.h>
#include <stdlib.h>
#include "block.h"
#include "fileDisk.h"

void sbTest() {
	struct sb *superBlock=malloc(sizeof(struct sb));;
	getSuper(superBlock);
	printf("********Test for superBlock attribute\n");
	printf("numDataBlock: %d\n", superBlock->numDataBlock);
	printf("numFreeDataBlock: %d\n", superBlock->numFreeDataBlock);
	printf("dataStart: %d\n", superBlock->dataStart);
	
	printf("numInodeBlock: %d\n", superBlock->numInodeBlock);
	printf("numFreeInodeBlock: %d\n", superBlock->numFreeInodeBlock);
	printf("inodeStart: %d\n", superBlock->inodeStart);
	

	/* This test is disgusting
	struct sb *newSuperBlock;
	newSuperBlock = malloc(sizeof(struct sb));
	printf("******Test for edit then write it back******\n");
	superBlock->dataStart = 666666;
	writeDisk(0, (void*)superBlock);
	readDisk(0, (void*)newSuperBlock);
	printf("new dataStart(666666): %d\n\n", newSuperBlock->dataStart);
	*/	
	free(superBlock);
}

void rootTest() {
	struct inode *root;
        root = malloc(sizeof(struct inode));
	
	printf("*****Test for root*******\n");
	readInode(1, (void*)root);
	printf("root type: %d\n", root->type);
	printf("root name: %s\n\n", root->name);
	
	free(root);
}

void testNextFree() {
	printf("*****Test for next free\n");
	printf("next free inode: %d\n", nextFreeBlock(0));
	printf("next free data: %d\n\n", nextFreeBlock(1));
}

void addElementTest() {
	struct inode *root;
        root = malloc(sizeof(struct inode));

	addElement(4, 1, 0, 1);
	readInode(1, (void*)root);
	
	printf("*****Test for add element******\n");
	printf("new size(1): %d\n", root->size);
	printf("element(4): %d\n\n", root->direct[0]);

	free(root);
}

void setBitTest() {
	setBit(2, 1);
	printf("******Testing setBit Test*****\nShould be 3\n");
	testNextFree();
}
/*
void freetest() {
	int *array = malloc(10*sizeof(int));
	free(array);
	printf("****free array succeed***\n");
	struct sb *superBlock = malloc(sizeof(struct sb));
	readDisk(0, superBlock);
	free(superBlock);
	printf("***free superBlock succeed***\n");
}
*/
int main() {
	my_mkfs();
	rootTest();
	testNextFree();
	addElementTest();
	sbTest();
	setBitTest();
	return 0;
}
