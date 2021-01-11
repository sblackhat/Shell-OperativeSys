
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "list.h"



int createList(MemoryList * L ){
	(*L) = (MemoryList) malloc (sizeof(tList));
	if (L != NULL){
	(*L)->lastpos = -1;
	return 1;
	}
	else{
	 perror("Impossible to allocate memory while creating the list.");
	return 0;}
}

int next(MemoryList L,int pos){
	if (pos < L -> lastpos){
		return pos++;
	}
	return -1;
}

int previous(MemoryList L,int pos){
	if ((pos < L -> lastpos) && (pos > 0)){
		return pos-1;
	}return -1;
}

int lastPos(MemoryList L){
	return L->lastpos;
}


int insertItem(MemoryList L,void * memAddress,unsigned int size, char * allocTime, tAlloc typeOfAllocation,
			   char * fileName,unsigned int fileDescriptor,unsigned int key){
	/* We always insert in the last position as the list has to follow a 
	FIFO behaviour when printing */
	int a = L-> lastpos+ 1;
	if (a < SIZE){
		L -> nodes[a].memAddress = memAddress;
		L -> nodes[a].size = size;
		L -> nodes[a].typeOfAllocation = typeOfAllocation;
		L->nodes[a].allocTime = strdup(allocTime);
		if ( fileName != NULL){
		L->nodes[a].fileName = strdup(fileName);}
		L -> nodes[a].fileDescriptor = fileDescriptor;
		L -> nodes[a].key = key;
		L -> lastpos++; 
		return 1;
	}
	else {
		perror("The list if full.\n");
        return 0;
	}
}

void deleteList(MemoryList * L){
	free(L);
	L = NULL;
}

char * solveTypeOfAlloc(int a){
	switch (a){
		case 0 : return "malloc";
		case 1 : return "mmap";
		case 2 : return "shared memory";
		default : return "unknown type";
	}
}

int findElementMalloc(MemoryList L, tAlloc tAlloc,unsigned int size){
	int lastpos = L->lastpos;
	for (int i = 0; i <= lastpos ; ++i)
	{
		if (L->nodes[i].size == size && L->nodes[i].typeOfAllocation == tAlloc ){
			return i;
		}
	}
	return -1;
}

int findElementMapped(MemoryList L, tAlloc tAlloc,char * fileName){
	int lastpos = L->lastpos;
	for (int i = 0; i <= lastpos ; ++i)
	{
		if (strcmp(L->nodes[i].fileName,fileName) == 0 && L->nodes[i].typeOfAllocation == tAlloc ){
			return i;
		}
	}
	return -1;
}

int findElementShared(MemoryList L, tAlloc tAlloc,unsigned int key){
	int lastpos = L->lastpos;
	for (int i = 0; i <= lastpos ; ++i)
	{
		if (L->nodes[i].key == key && L->nodes[i].typeOfAllocation == tAlloc ){
			return i;
		}
	}
	return -1;
}

void delete(MemoryList L,int pos){
	// Free strings
	free(L->nodes[pos].allocTime);
	L->nodes[pos].allocTime = NULL;
	free(L->nodes[pos].fileName);
	L->nodes[pos].fileName = NULL;
	
	if( (pos != -1) && (L->lastpos != -1) ){
		for (int i = pos; i <= L->lastpos; ++i)
		{
			L->nodes[i] = L->nodes[i+1];
		}
	}
	else printf(" Element not found !\n");
	L->lastpos--;
}

void deallocMmap(MemoryList L, int pos){
	if( (munmap(L->nodes[pos].memAddress, L->nodes[pos].size) == -1) || (close (L -> nodes[pos].fileDescriptor) == -1) )
    {
        perror("Not possible to deallocate mmap.\n");
        return;
     }
    printf ("Block at address %p deallocated (mmap).\n", L->nodes[pos].memAddress);
    return;
    delete(L, pos);
}

void deallocShared(MemoryList L, int pos){
	shmdt (L->nodes[pos].memAddress);
    printf("Block at address %p deallocated (shared memory).\n", L->nodes[pos].memAddress);
    delete(L, pos);
}

void deleteElement(MemoryList L , tAlloc typeOfAllocation,unsigned int size,char * fileName,unsigned int key){
  int pos;
	switch (typeOfAllocation){
		case 0 : if ( (pos = findElementMalloc(L,typeOfAllocation,size) )!= -1){ delete(L, pos);}
				 break;
	 	case 1 : if ( (pos = findElementMapped(L,typeOfAllocation,fileName) )!= -1){ deallocMmap(L, pos);}
	 			 break;
	 	case 2 : if ( (pos = findElementShared(L,typeOfAllocation,key) )!= -1) { deallocShared(L, pos);}
	 			 break;
	    default : pos = -1;
	    		  printf("Type of allocation not valid ! \n");
				 break;
	 }
}


void printSimple(MemoryList L){
	char * tAlloc;
	tNode actualNode;
	for (int i = 0; i <= L-> lastpos; ++i)
	{   actualNode = L->nodes[i];
		tAlloc = solveTypeOfAlloc(actualNode.typeOfAllocation);
		printf("%18p: size:%d %s %s\n", L->nodes[i].memAddress,L->nodes[i].size
								, tAlloc,L->nodes[i].allocTime);
	}
}

void printMalloc(MemoryList L){
	tNode actualNode;
	for (int i = 0; i <= L-> lastpos; ++i)
	{   actualNode = L->nodes[i];
		if (actualNode.typeOfAllocation == 0){
		printf("%18p: size:%d %s %s \n", actualNode.memAddress,actualNode.size
								, "malloc",actualNode.allocTime);
		}
	}
}

void printMmap(MemoryList L){
	tNode actualNode;
	for (int i = 0; i <= L-> lastpos; ++i)
	{   actualNode = L->nodes[i];
		if (actualNode.typeOfAllocation == 1){
		printf("%18p: size:%d %s %s (fd:%d) %s\n", actualNode.memAddress,actualNode.size
								, "mmap",actualNode.fileName,actualNode.fileDescriptor,actualNode.allocTime);
		}
	}
}

void printShared(MemoryList L){
	tNode actualNode;
	for (int i = 0; i <= L-> lastpos; ++i)
	{   actualNode = L->nodes[i];
		if (actualNode.typeOfAllocation == 2){
		printf("%18p: size:%d %s %s (key %d) %s\n", actualNode.memAddress,actualNode.size
								, "shared memory",actualNode.fileName,actualNode.key,actualNode.allocTime);
		}
	}
}

int findAddress (MemoryList L, void * p){
	int i;
	for (i = 0; i <= L->lastpos; i++){
		if (L->nodes[i].memAddress == p){
			return i;
		}
	}
	//That address is not allocated.
	return -1;
}
