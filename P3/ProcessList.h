#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct NODEINFO{
	pid_t pid;
	int priority;
	char * time;
}tInfo;

typedef struct NODE{
	tInfo nodeinfo;
	struct NODE * next;
}tProcessNode;

typedef struct LIST{
	tProcessNode node;
	int lastpos;
}tProcessList;

typedef tProcessList * PList;

int createPList (PList * L);
tInfo constructNodeP (pid_t pid, int prio, char * time);
int insertP (PList * L, pid_t pid, int prio, char * time);
void printElementP (tInfo node);
void printPList (PList L);
void printPid (PList L, pid_t pid);
void deleteElementP (PList L, tProcessNode * node);
void deleteElementByState (PList L, char * pstate);
void deleteListP(PList P);