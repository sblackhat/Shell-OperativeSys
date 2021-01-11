
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ProcessList.h"
#include "sgfuns.h"


/*Shows the list of background processes of the shell. For each process it
must show (IN A SINGLE LINE):
– The process pid
– The process priority
– The command line the process is executing (executable and argu-
ments)
– The time it started
– The process state (Running, Stopped, Terminated Normally or
Terminated By Signal).
– For processes that have terminated normally the value returned,
for processes stopped or terminated by a signal, the name of the
signal.
This command USES THE LIST OF BACKGROUND PRO-
CESSES of the shell, it DOES NOT HAVE TO GO THROUGH
THE /proc FILESYSTEM*/

int createPList (PList * L){
	(*L) = malloc (sizeof(tProcessList));
	if (L != NULL){
		(*L)->lastpos = -1;
		(*L)->node.next = NULL;
		return 1;
	}
	return -1;
}

int solveState (pid_t pid){
	int status;
	waitpid(pid, &status, WNOHANG | WUNTRACED);    
    if (WIFSIGNALED(status)){
    	return WTERMSIG(status);
    }
	else if (WIFSTOPPED(status)){
    	return WSTOPSIG(status);
    }
    else {return 000;}
    
}

char * solveStateC (pid_t status){
			  if (WIFEXITED(status))
                 return "FINISH";
              else if (WIFSIGNALED(status))
                 return "SIGNAL";
              else if (WIFSTOPPED(status))
                 return "STOP";
             else return "ACTIVE";
              
           }

tInfo constructNodeP (pid_t pid, int prio, char * time){
	tInfo aux;
	int i = 0;
	aux.pid = pid;
	aux.priority = prio;
	aux.time = strdup(time);
	return aux;
}

int insertP (PList * L, pid_t pid, int prio, char * time){
	
	if (L == NULL) return -1;
	tInfo info = constructNodeP (pid, prio, time);
	if ((*L)-> lastpos == -1){
		(*L)->node.nodeinfo = info;
		(*L)->node.next = NULL;
	}
	else{

		tProcessNode * p = (tProcessNode *) malloc (sizeof(tProcessNode));
		if (p == NULL) return -1;
		p->nodeinfo = info;
		p->next = NULL;
		tProcessNode * aux = &(*L)->node;

		while (aux->next != NULL){
			aux = aux->next;
		}
		aux->next = p;
	}
	(*L)->lastpos++;
	return 1;     
}


void printElementP (tInfo node){
	printf("%d P = %d %s (%03d) %s ", 
		node.pid, node.priority, node.time, solveState(node.pid), solveStateC(node.pid));
    printf("\n");
}

void printPList (PList L){
	if (L != NULL){
		tProcessNode * p = &L->node;
		while (p != NULL){
			printElementP((*p).nodeinfo);
			p = p->next;
		}
	}else{
	perror("Nothing to print the List is empty");
	return;}
}

void printPid (PList L, pid_t pid){
	if (L != NULL){
		tProcessNode * p = &L->node;
		while (p != NULL){
			if ( (*p).nodeinfo.pid == pid){
			printElementP((*p).nodeinfo);
			return;
			}
			p = p->next;
		}
	}else{
	perror("Nothing to print the List is empty");
	return;}
}

void deleteElementP (PList L, tProcessNode * node){
	if (L != NULL){
		tProcessNode * p = &L->node;
		while (p->next != NULL){
			if ( &p->next == &node)
			break;
			p = p->next;
		}			
			if (p->next == NULL)
			  return;
			p->next = node->next;
		free(node);
		node = NULL;
	}else{
	perror("Nothing to delete the List is empty");
	return;}
}

void deleteElementByState (PList L, char * pstate){
	if (L != NULL){
		tProcessNode * p = &L->node;
		while (p != NULL){
			if ( strcmp(pstate, solveStateC(p->nodeinfo.pid)) == 0){
			deleteElementP(L, p);
			return;
			}
			p = p->next;
		}
	}else{
	perror("Nothing to delete the List is empty");
	return;}
}

void deleteListP(PList P){
	tProcessNode * p = &P->node;
	while(p != NULL){
		deleteElementP(P,p);
		tProcessNode * p = &P->node;
		}
}
/*
int main(){
	PList L;
	createPList(&L);
	pid_t i = 11558;
	insertP(&L,i,1,"TIEMPO");
	insertP(&L,i,1,"TIEMPO");
	insertP(&L,i,1,"TIEMPO");
	//insertP(&L,node2);
	printPList(L);
	//deleteListP(L);
	return 0;
}
*/

