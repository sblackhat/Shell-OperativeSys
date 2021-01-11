

#define SIZE 4096

typedef enum typeOfAllocationE {Malloc,Mapped,Shared}tAlloc;

typedef struct Node{
	//Common atributtes
	void * memAddress;
	unsigned int size;
	char * allocTime;
	tAlloc typeOfAllocation;
	//Mmap
	char * fileName;
	unsigned int fileDescriptor;
	//Shared key_t 
	unsigned int key;
}tNode;

typedef struct 
{
	tNode nodes[SIZE];
	int lastpos;
}tList;

 typedef tList * MemoryList;


int createList(MemoryList * L );
int next(MemoryList L,int pos);
int previous(MemoryList L,int pos);
int lastPos(MemoryList L);
int insertItem(MemoryList L,void * memAddress,unsigned int size, char * allocTime,
			 tAlloc typeOfAllocation,char * fileName, unsigned int fileDescriptor,unsigned int key);
void deleteList(MemoryList * L);
int findElement(MemoryList L, tAlloc tAlloc,int size);
int findElementMapped(MemoryList L, tAlloc tAlloc,char * fileName);
int findElementShared(MemoryList L, tAlloc tAlloc,unsigned int key);
void deleteElement(MemoryList L , tAlloc typeOfAllocation,unsigned int size,char * fileName,unsigned int key);
void delete(MemoryList L,int pos);
char * solveTypeOfAlloc(int a);
int findAddress (MemoryList L, void * p);
void delete(MemoryList L,int pos);
void deallocMmap(MemoryList L, int pos);
void deallocShared(MemoryList L, int pos);
void printSimple(MemoryList L);
void printMalloc(MemoryList L);
void printShared(MemoryList L);
void printMmap(MemoryList L);
